#pragma once
#include <LiveData/livedata.h>

#define PORT        80

void initWebServer();
void routes();

void handleClient();

void getLivedata();
