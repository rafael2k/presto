// -*- Mode: c++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4; c-file-style:"stroustrup" -*-
// Copyright (C) 2002, Morten Stenshorne

// Used in the Opera web browser with permission.

#ifndef __AUDIOPLAYER_H__
#define __AUDIOPLAYER_H__

/**
 * This is the class that takes care of playing the sounds.
 * It implements everything that is cross-platform. The rest
 * is defined as pure virtual methods which have to be reimplemented
 * in a platform/device specific class.
 */
class AudioPlayer
{
private:
	bool stop;
	class AudioPlayerItem *first;
	int mixerrate;
	bool autoclose;

	int volume;
	bool fade;
	int fade_delta;
	int fade_destination;
	int fade_duration;
	double fade_counter;
	double fade_samples_per_step;

	int output_level;

protected:
	/**
	 * Returns true if a mixer termination has been requested.
	 * Platform reimplementations of this class should probably
	 * call this method regularly, to see if it's about time to
	 * stop.
	 */
	bool StopInProgress();

	/**
	 * Returns whether autoclose is enabled or not. See SetAutoCloseDevice().
	 */
	bool AutoCloseDevice();

	/**
	 * Open the sound device and start a thread or something to run the 
	 * mixer loop (call SoundMixer()) (if it was opened successfully).
	 *
	 * @return true if successful or if already open. Return false
	 * if something failed.
	 */
	virtual bool OpenDevice() = 0;

public:
	AudioPlayer();

	/**
	 * Destructor. It should be reimplemented.
	 * You probably want to call StopMixer() and CloseDevice() then.
	 */
	virtual ~AudioPlayer();

	/**
	 * Lock the object. This is necessary to implement if the 
	 * sound implementation creates threads or processes.
	 * Locking a mutex might be the appropriate thing to do when
	 * this method is called then. Most methods in this class
	 * might call Lock().
	 */
	virtual void Lock() = 0;

	/**
	 * Unlock the object. This is necessary to implement if the 
	 * sound implementation creates threads or processes.
	 * Locking a mutex might be the appropriate thing to do when
	 * this method is called then. Most methods in this class
	 * might call Unlock().
	 */
	virtual void Unlock() = 0;

	/**
	 * Perform a cleanup. Close streams that have finished playing.
	 * This method is called each time Play() or Stop() is called.
	 * If this isn't sufficient, call it yourself at other times.
	 */
	void CleanUp();

	/**
	 * Setting this one to 'true' will cause the device to be closed
	 * when all sounds have finished playing. This feature has to be
	 * implemented in a subclass. When no sounds are playing anymore
	 * (e.g. when SoundMixer() returns 0), the device should be closed.
	 */
	void SetAutoCloseDevice(bool autoclose);

	/**
	 * The sound mixer itself. The endianness of the data generated by this
	 * method is the same as the endianness on the system.
	 *
	 * @param data result (if any) of the sound mixer will be put here
	 * @param tmpbuf temporary buffer needed by the mixer. Should
	 * have the same size as the data buffer. It's done this way to avoid
	 * thread safety problems.
	 * @param bufsize size (in bytes) of the buffer
	 * @return number of bytes ready for playback. If no sounds are 
	 * currently playing, the contents of 'data' is undefined and
	 * 0 is returned.
	 */
	int SoundMixer(unsigned char *data, unsigned char *tmpbuf, int bufsize);

	/**
	 * Play a sound. OpenDevice() will be called, to ensure that the
	 * device is open. Note the new AudioPlayerItem object created here 
	 * takes ownership of the AudioStream pointer, meaning that it will
	 * take care of its deletion. So don't delete it yourself. Don't 
	 * allocate it on the stack!
	 * 
	 * @param s the audio stream to play
	 * @param loops number of times to repeat the sound. 0 means forever
	 * @param id If not NULL, this will be set to a handle for the
	 * AudioPlayerItem. A value of 0 means that something failed, -1
	 * means allocation error.
	 * @return true if it was successful, false if something failed.
	 */
	bool Play(class AudioStream *s, int loops=1, int *id = 0);

	/**
	 * Stop playing a sound
	 * @param id the id of the sound to stop
	 * @return true if successful, false if id not found
	 */
	bool Stop(int id);

	/**
	 * Stop all sounds and request mixer termination.
	 */
	void StopMixer();

	/**
	 * Return the current mixerrate.
	 */
	int MixerRate() { return mixerrate; }

	/**
	 * Request the sound to fade in or out to a specific destination
	 * volume. If a fade effect already is in progress, it will
	 * be aborted, replaced with this new fade effect.
	 *
	 * @param duration duration of the fade effect, specified in 
	 * milliseconds.
	 * @param destination the destination volume that will be
	 * achieved when fading is finished. A value of 0 is silent,
	 * while 1000 is maximum volume.
	 */
	void Fade(int destination, int duration);

	/**
	 * Sets the general volume. This will cancel any fade effect that
	 * is in progress.
	 * @param new volume. 0 is silent, 1000 is max.
	 */
	void SetVolume(int vol);
};

#endif // __AUDIOPLAYER_H__
