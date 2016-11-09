int sendMessage(char slaveAddress, char slaveRegister, char dataByte);
int repeatedSendMessage(char slaveAddress, char firstSlaveRegister, char dataBytes[], int length);
int receiveMessage(char slaveAddress, char slaveRegister);
int repeatedReceiveMessage(char slaveAddress, char slaveRegister, int* receivedBytes, int times);
