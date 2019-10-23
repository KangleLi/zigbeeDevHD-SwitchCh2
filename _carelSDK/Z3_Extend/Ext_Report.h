
#ifndef CARELSDK_Z3_EXTEND_EXT_REPORT_H_
#define CARELSDK_Z3_EXTEND_EXT_REPORT_H_





void _reportStdAttribute(uint16_t dstNodeId, uint8_t srcEp, uint8_t dstEp, \
						 uint16_t clusterId, uint16_t attrId, uint8_t c_s);

void _reportMfgServerAttribute(uint16_t dstNodeId, uint8_t srcEp, uint8_t dstEp, \
						uint16_t clusterId, uint16_t attrId);

#endif /* CARELSDK_Z3_EXTEND_EXT_REPORT_H_ */
