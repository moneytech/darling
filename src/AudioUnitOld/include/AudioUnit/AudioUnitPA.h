#ifndef AUDIOUNITPA_H
#define	AUDIOUNITPA_H
#include "AudioOutputUnitComponent.h"
#include <pulse/pulseaudio.h>
#include <thread>

#ifndef kOutputBus
#	define kOutputBus		0 // playback
#	define kInputBus		1 // recording
#endif

class AudioUnitPA : public AudioOutputUnitComponent
{
public:
	AudioUnitPA();
	virtual ~AudioUnitPA();
	
	OSStatus init() override;
	OSStatus deinit() override;

	OSStatus start() override;
	OSStatus stop() override;
	
	OSStatus reset(AudioUnitScope inScope, AudioUnitElement inElement) override;
	
	OSStatus render(AudioUnitRenderActionFlags *ioActionFlags, const AudioTimeStamp *inTimeStamp, UInt32 inBusNumber, UInt32 inNumberFrames, AudioBufferList *ioData) override;
	
	int cardIndex() const override;
private:
	OSStatus initOutput();
private:
	static void initializePA();
	static void deinitializePA();
	static void paContextStateCB(pa_context* c, void*);
	static void paStreamStateCB(pa_stream* s, void*);
	static void paStreamWriteCB(pa_stream* s, size_t length, void*);
	static std::string getAppName();
	pa_sample_spec paSampleSpecForASBD(const AudioStreamBasicDescription& asbd);
	
	void requestDataForPlayback(size_t length);
	
	inline bool isOutputPlanar() const { return m_config[kOutputBus].second.mFormatFlags & kAudioFormatFlagIsNonInterleaved; }
	inline bool isInputPlanar() const { return m_config[kInputBus].first.mFormatFlags & kAudioFormatFlagIsNonInterleaved; }
	
	OSStatus renderOutput(AudioUnitRenderActionFlags *ioActionFlags, const AudioTimeStamp *inTimeStamp, UInt32 inNumberFrames, AudioBufferList *ioData);
	OSStatus renderInterleavedOutput(AudioUnitRenderActionFlags *ioActionFlags, const AudioTimeStamp *inTimeStamp, UInt32 inNumberFrames, AudioBufferList *ioData);
	OSStatus renderPlanarOutput(AudioUnitRenderActionFlags *ioActionFlags, const AudioTimeStamp *inTimeStamp, UInt32 inNumberFrames, AudioBufferList *ioData);
	
	OSStatus renderInput(AudioUnitRenderActionFlags *ioActionFlags,const AudioTimeStamp *inTimeStamp, UInt32 inNumberFrames, AudioBufferList *ioData);
	OSStatus renderInterleavedInput(AudioUnitRenderActionFlags *ioActionFlags,const AudioTimeStamp *inTimeStamp, UInt32 inNumberFrames, AudioBufferList *ioData);
	OSStatus renderPlanarInput(AudioUnitRenderActionFlags *ioActionFlags,const AudioTimeStamp *inTimeStamp, UInt32 inNumberFrames, AudioBufferList *ioData);
private:
	static pa_mainloop* m_mainloop;
	static pa_context* m_context;
	static std::thread* m_mainloopThread;
private:
	pa_stream* m_stream;
	
	// PulseAudio supports only unsigned 8-bit PCM and only signed >8-bit PCM.
	// We need to perform conversion of samples if input/output differs.
	bool m_convertUnsignedSigned = false;
	bool m_started = false;
};

#endif	/* AUDIOUNITPA_H */

