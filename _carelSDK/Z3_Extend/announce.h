
#ifndef CARELSDK_ANNOUNCE_H_
#define CARELSDK_ANNOUNCE_H_




#define _ANNOUNCE_CLUSTER_ID			0x0013
#define _ANNOUNCE_PROFILE_ID			0x0000
#define _ANNOUNCE_SOURCE_EP				0x00
#define _ANNOUNCE_DESTINATION_EP		0x00
#define _ANNOUNCE_SEQNUM				131
// 0xFFFF ȫ�����豸
// 0xFFFE ��ȡbinding table��ȡ�̵�ַ
// 0xFFFD ���˽��յ��豸
// 0xFFFC Э������·����
#define _ANNOUNCE_DESTINATION_NODEID	0xFFFD

// �������� ������0	Alternate PAN Coordinator Capability : No
// �������� ����1��	Device Type : Router
// �������� ��1����	Power Source : Mains-Powered Device
// �������� 1������	Receiver On When Idle : Yes
// ����00 ��������	Reserved
// ��0���� ��������	Security Capability : Standard Security Mode
// 1������ ��������	Allocate Address : 16-bits Network Address
#define _ANNOUNCE_CAPAINFO				0x8E




extern int8_t _carelDeviceAnnounce(void);

#endif /* CARELSDK_ANNOUNCE_H_ */
