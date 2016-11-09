#pragma once

int state;

int init(void);

int start(void);

int restart(void);

int stop(void);

int send(char byte);

int recive(void);

int generateACK(int typeACK);
