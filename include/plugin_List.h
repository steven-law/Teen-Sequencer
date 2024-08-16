
// plugin.h
#ifndef PLUGIN_LIST_H
#define PLUGIN_LIST_H

#include <Arduino.h>
#include "project_variables.h"
#include <pluginClass.h>
#include <Plugin_1.h>
#include <Plugin_2.h>
#include <Plugin_3.h>
#include <Plugin_4.h>
#include <Plugin_5.h>
#include <Plugin_6.h>
#include <Plugin_7.h>
#include <Plugin_8.h>


extern Plugin_1 plugin_1;
extern Plugin_2 plugin_2;
extern Plugin_3 plugin_3;  // Deklaration der Variable
extern Plugin_4 plugin_4;
extern Plugin_5 plugin_5;
extern Plugin_6 plugin_6;
extern Plugin_7 plugin_7;
extern Plugin_8 plugin_8;
extern PluginControll *allPlugins[NUM_PLUGINS];



#endif // PLUGIN_LIST_H