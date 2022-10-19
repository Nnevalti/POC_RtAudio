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

int tick( void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames,
           double streamTime, RtAudioStreamStatus status, void *data )
{
  // Since the number of input and output channels is equal, we can do
  // a simple buffer copy operation here.
  if ( status ) std::cout << "Stream over/underflow detected." << std::endl;

  unsigned int *bytes = (unsigned int *) data;
  memcpy( outputBuffer, inputBuffer, *bytes );
  return 0;
}

int main(int ac, char **av)
{
    RtAudio dac;

    dac.showWarnings( true );

    if (dac.getDeviceCount() == 0)
    {
        cout << "No device found exiting" << endl;
        exit(0);
    }

    // Device Informations
    print_RtAudio(dac);

    int inputId = 2;
    int outputId = 0;
    // cout << "Select input device: " << endl;
    // cin >> inputId;
    // cout << "Select output device: " << endl;
    // cin >> outputId;

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

    options.flags = RTAUDIO_NONINTERLEAVED;
    try {
        auto error_callback = [](auto flag, auto msg){
            std::cerr << "RtAudio Error: " << msg << std::endl;
        };

        // int data;
        double *data = (double *) calloc( 2, sizeof( double ) );
        dac.openStream(&outputParameters, NULL, format, sampleRate, &bufferFrames, &tick, (void *)data, &options, error_callback);
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