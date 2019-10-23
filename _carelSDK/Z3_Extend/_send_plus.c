#include "../common.h"

#define INVALID_MESSAGE_TAG 0xFF


// 解除限制版信息发送
EmberStatus _send_plus(EmberOutgoingMessageType type,
                        uint16_t indexOrDestination,
                        EmberApsFrame *apsFrame,
                        uint16_t messageLength,
                        uint8_t *message,
                        bool broadcast,
                        EmberNodeId alias,
                        uint8_t sequence)
{
  EmberStatus status;
  uint8_t messageTag = INVALID_MESSAGE_TAG;

  if (messageLength < EMBER_AF_ZCL_OVERHEAD) {
    return EMBER_ERR_FATAL;
  } else if ((message[0] & ZCL_MANUFACTURER_SPECIFIC_MASK) != 0U) {
    if (messageLength < EMBER_AF_ZCL_MANUFACTURER_SPECIFIC_OVERHEAD) {
      return EMBER_ERR_FATAL;
    }
  }

  {
    EmberAfMessageStruct messageStruct = {
      NULL,
      apsFrame,
      message,
      indexOrDestination,
      messageLength,
      type,
      broadcast,
    };
    if (emberAfPreMessageSendCallback(&messageStruct,
                                      &status)) {
      return status;
    }
  }

  // SE 1.4 requires an option to disable APS ACK and Default Response
//  emAfApplyDisableDefaultResponse(&message[0]);
  extern void emAfApplyRetryOverride(EmberApsOption *options);
  emAfApplyRetryOverride(&apsFrame->options);

  if (messageLength
      <= emberAfMaximumApsPayloadLength(type, indexOrDestination, apsFrame)) {
	  extern EmberStatus emAfSend(EmberOutgoingMessageType type,
              uint16_t indexOrDestination,
              EmberApsFrame *apsFrame,
              uint8_t messageLength,
              uint8_t *message,
              uint8_t *messageTag,
              EmberNodeId alias,
              uint8_t sequence);
    status = emAfSend(type,
                      indexOrDestination,
                      apsFrame,
                      (uint8_t)messageLength,
                      message,
                      &messageTag,
                      alias,
                      sequence);
  } else {
    status = EMBER_MESSAGE_TOO_LONG;
  }

  if (status == EMBER_OPERATION_IN_PROGRESS
      && apsFrame->options & EMBER_APS_OPTION_DSA_SIGN) {
    // We consider "in progress" signed messages as being sent successfully.
    // The stack will send the message after signing.
    status = EMBER_SUCCESS;
    extern void emAfSetCryptoStatus(uint8_t newStatus);
    emAfSetCryptoStatus(1);
  }

  if (status == EMBER_SUCCESS) {
    emberAfAddToCurrentAppTasks(EMBER_AF_WAITING_FOR_DATA_ACK
                                | EMBER_AF_WAITING_FOR_ZCL_RESPONSE);
  }

  emberAfPopNetworkIndex();
  return status;
}
