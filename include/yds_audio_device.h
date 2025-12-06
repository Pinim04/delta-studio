#ifndef YDS_AUDIO_DEVICE_H
#define YDS_AUDIO_DEVICE_H

#include "yds_audio_system_object.h"
#include "yds_audio_parameters.h"

#include "yds_window.h"
#include <cstring>

class ysAudioSource;
class ysAudioBuffer;

class ysAudioDevice : public ysAudioSystemObject {
    friend class ysAudioSystem;

public:
    static constexpr int MaxDeviceNameLength = 256;

public:
    ysAudioDevice();
    ysAudioDevice(API api);
    ~ysAudioDevice();

    bool IsConnected() const { return m_connected; }
    void SetDeviceName(const char *newName) { 
        #if defined(_MSC_VER)
            strcpy_s(m_deviceName, MaxDeviceNameLength, newName);
        #else
            std::strncpy(m_deviceName, newName, MaxDeviceNameLength - 1);
            m_deviceName[MaxDeviceNameLength - 1] = '\0';
        #endif
    }

    virtual ysAudioBuffer *CreateBuffer(const ysAudioParameters *parameters, SampleOffset size) = 0;

    virtual ysAudioSource *CreateSource(const ysAudioParameters *parameters, SampleOffset size) = 0;
    virtual ysAudioSource *CreateSource(ysAudioBuffer *sourceBuffer) = 0;

    virtual void UpdateAudioSources() = 0;

    virtual ysError DestroyAudioBuffer(ysAudioBuffer *&buffer);
    virtual ysError DestroyAudioSource(ysAudioSource *&source);
    ysError DestroyAudioBuffers();

protected:
    char m_deviceName[MaxDeviceNameLength];
    ysWindow *m_windowAssociation;

    ysDynamicArray<ysAudioBuffer, 4> m_audioBuffers;
    ysDynamicArray<ysAudioSource, 4> m_audioSources;

    bool m_connected;
};

#endif /* YDS_AUDIO_DEVICE_H */
