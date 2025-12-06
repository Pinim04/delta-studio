#ifndef YDS_DS8_SYSTEM_H
#define YDS_DS8_SYSTEM_H

#include "yds_audio_system.h"
#include "yds_window.h"

#ifdef _WIN32
#include <dsound.h>
#endif

class ysDS8Device;

class ysDS8System : public ysAudioSystem {
public:
    ysDS8System();
    virtual ~ysDS8System();

    virtual void EnumerateDevices();
    virtual void ConnectDevice(ysAudioDevice *device, ysWindow *windowAssociation);
    virtual void ConnectDeviceConsole(ysAudioDevice *device);
    virtual void DisconnectDevice(ysAudioDevice *device);

protected:
    ysDS8Device *AddDS8Device();

    static BOOL CALLBACK DirectSoundEnumProc(LPGUID lpGUID, LPCTSTR lpszDesc, LPCTSTR lpszDrvName, LPVOID lpContext);
};

#endif /* YDS_DS8_SYSTEM_H */
