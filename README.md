# AudioPerfTest

A small project for UE to allow performance comparisons between UE's built in audio libraries (Ogg) and MP3 via AvAudioPlayer.

## Usage

Load into a level (stat unit should be shown by default) and trigger Ogg or Av playback by shooting at the rocks. 3s after the last shot the view will be reset to default.

The "Burn CPU" object adds 10ms to the CPU and GPU threads.

## Notes:

There appears to be a bug in UE 4.18 where non-UFS files (in this case the mp3s) are not deployed to the device. T

To workaround this-

* After deployment, download the app container using Xcode
* Open the container using "Show Package Contents"
* copy the mp3s folder from AudioPerfTest/Content to <container>/AppData/Library/Caches/audioperftest/content

The project has only been compiled for Mac/iOS, the UAVAudioPlaybackComponent will need some things #ifdef'd out for other platforms 
