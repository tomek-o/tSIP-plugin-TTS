# tSIP-plugin-TTS
Text-To-Speech plugin for tSIP softphone.
This plugin lets you use typed text instead of microphone for audio input, adding chat-like window.

## Installation
- get tSIP 0.01.66.06 or newer version
- copy TTS.dll to tSIP \phone subdirectory
- start tSIP
- in tSIP Settings/Calls uncheck "Disconnect call on audio error" (otherwise call would be disconnected after first TTS sentence)
- activate plugin using tSIP plugin list in Settings/Phones (plugins)
- test it, e.g. using other softphone instance and peer-to-peer calling

## Usage
- TTS.dll calls Lua scripts from its configuration on load/unload to switch audio source to silence/nullaudio and winwave respectively
- to get rid of TTS window - deactivate plugin
- you might use tSIP programmable keys (and/or associated global hotkeys) for switching to winwave (normal microphone) during the call

## Compiling
- plugin source directory should be placed next to tSIP main directory - plugin is using interface header and jsoncpp library from tSIP project group