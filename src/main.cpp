#include <iostream>
#include "../include/RtAudio.h"

using namespace std;

void print_RtAudio(RtAudio dac)
{
    cout << "Number of device detected: " << dac.getDeviceCount() << endl;
    cout << "Default device is: " << dac.getDeviceInfo(dac.getDefaultOutputDevice()).name << endl;
    cout << endl;
    cout << "List of device: " << endl;
    for (int i = 0; i < dac.getDeviceCount(); i++)
    {
        RtAudio::DeviceInfo device = dac.getDeviceInfo(i);
        cout << "--------------------------------------------------------" << endl;
        cout << "Name = " << device.name << ", id = " << i << ":" << endl;
        cout << "Max Input Channels = " << device.inputChannels << ", Max Output Channels = " << device.outputChannels << ":" << endl;
        cout << "Supported sample rate: ";
        for (auto it = device.sampleRates.begin(); it != device.sampleRates.end(); it++)
            cout << *it << (it != device.sampleRates.end() - 1 ? ", " : "");
        cout << endl;
        cout << "Prefered sample rate: " << device.preferredSampleRate << endl;
        cout << endl;
    }
}

int tick(void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames, double streamTime, RtAudioStreamStatus status, void *data)
{

}

int main(int ac, char **av)
{
    RtAudio dac;

    if (dac.getDeviceCount() == 0)
    {
        cout << "No device found exiting code: 0" << endl;
        return 0;
    }

    // Device Informations
    print_RtAudio(dac);

    int inputId;
    int outputId;
    cout << "Select input device: " << endl;
    cin >> inputId;
    cout << "Select output device: " << endl;
    cin >> outputId;

    cout << inputId << " " << outputId << endl;

    // Setup an audio stream
    RtAudio::StreamParameters inputParameters;
    RtAudio::StreamParameters outputParameters;
    RtAudio::StreamOptions options;
    RtAudioFormat format = sizeof(float) == 8 ? RTAUDIO_FLOAT64 : RTAUDIO_FLOAT32;

    inputParameters.deviceId = inputId;
    inputParameters.nChannels = 2;

    outputParameters.deviceId = outputId;
    outputParameters.nChannels = 2;

    unsigned int sampleRate = 44100;
    unsigned int bufferFrames = 16;

    // options.flags = RTAUDIO_NONINTERLEAVED;
    try {
        dac.openStream( &inputParameters, &outputParameters, format, sampleRate, &bufferFrames, &tick, NULL, &options);
        dac.startStream();
    }
        catch ( RtAudioError& e ) {
        std::cout << '\n' << e.getMessage() << '\n' << std::endl;
        exit( 0 );
    }

    char input;
    cout << "\nPlaying ... press <enter> to quit.\n";
    cin.get( input );
    try
    {
        // Stop the stream
        dac.stopStream();
    }
    catch (RtAudioError& e)
    {
        e.printMessage();
    }

    if (dac.isStreamOpen()) dac.closeStream();

    return 0;
}