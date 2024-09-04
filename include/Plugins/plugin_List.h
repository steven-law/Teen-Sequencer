
// plugin.h
#ifndef PLUGIN_LIST_H
#define PLUGIN_LIST_H

#include <Arduino.h>
#include "project_variables.h"
#include <Plugins/pluginClass.h>
#include <Plugins/Plugin_1.h>
#include <Plugins/Plugin_2.h>
#include <Plugins/Plugin_3.h>
#include <Plugins/Plugin_4.h>
#include <Plugins/Plugin_5.h>
#include <Plugins/Plugin_6.h>
#include <Plugins/Plugin_7.h>
#include <Plugins/Plugin_8.h>
#include <Plugins/Plugin_9.h>
#include <Plugins/Plugin_10.h>


extern Plugin_1 plugin_1;
extern Plugin_2 plugin_2;
extern Plugin_3 plugin_3;  // Deklaration der Variable
extern Plugin_4 plugin_4;
extern Plugin_5 plugin_5;
extern Plugin_6 plugin_6;
extern Plugin_7 plugin_7;
extern Plugin_8 plugin_8;
extern Plugin_9 plugin_9;
extern Plugin_10 plugin_10;
extern PluginControll *allPlugins[NUM_PLUGINS];



#endif // PLUGIN_LIST_H