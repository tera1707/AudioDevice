using Windows.Win32.Foundation;
using Windows.Win32.Media.Audio;
using Windows.Win32.System.Com.StructuredStorage;
using Windows.Win32.UI.Shell.PropertiesSystem;
using Windows.Win32.System.Com;
using Windows.Win32.Media.Audio.Endpoints;

namespace MicJikkenCsWithCsWin32;

internal class Program
{
    static void Main(string[] args)
    {
        var pEnum = new MMDeviceEnumerator() as IMMDeviceEnumerator;

        var pNotifClient = new CMMNotificationClient();

        pEnum!.RegisterEndpointNotificationCallback(pNotifClient);

        pEnum!.EnumAudioEndpoints(EDataFlow.eCapture, DEVICE_STATE.DEVICE_STATE_ACTIVE, out var pCollection);

        pCollection!.GetCount(out var deviceCount);

        for (uint i = 0; i < deviceCount; i++)
        {
            pCollection.Item(i, out var pEndpoint);

            pEndpoint.OpenPropertyStore(STGM.STGM_READ, out var pProperties);

            unsafe
            {
                var PKEY_Device_FriendlyName = new PROPERTYKEY() { fmtid = new Guid(0xa45c254e, 0xdf1c, 0x4efd, 0x80, 0x20, 0x67, 0xd1, 0x46, 0xa8, 0x50, 0xe0), pid = 14 };
                pProperties.GetValue(&PKEY_Device_FriendlyName, out var vName);

                Console.WriteLine(vName.Anonymous.Anonymous.Anonymous.pwszVal);

                Guid IID_IAudioEndpointVolume = typeof(IAudioEndpointVolume).GUID;
                pEndpoint.Activate(&IID_IAudioEndpointVolume, 0, (PROPVARIANT_unmanaged*)0 ,out var endpointVolume);
                var masterVol = endpointVolume as IAudioEndpointVolume;

                var guid = new Guid();
                masterVol!.GetMasterVolumeLevelScalar(out var pMasterVolumeLevel);
                masterVol!.SetMasterVolumeLevelScalar(0.80f, &guid);
            }
        }

        Console.ReadLine();
    }

    public class CMMNotificationClient : IMMNotificationClient
    {
        public void OnDeviceStateChanged(PCWSTR pwstrDeviceId, DEVICE_STATE dwNewState)
        {
            Console.WriteLine(pwstrDeviceId);
        }

        public void OnDeviceAdded(PCWSTR pwstrDeviceId)
        {
            Console.WriteLine(pwstrDeviceId);
        }

        public void OnDeviceRemoved(PCWSTR pwstrDeviceId)
        {
            Console.WriteLine(pwstrDeviceId);
        }

        public void OnDefaultDeviceChanged(EDataFlow flow, ERole role, PCWSTR pwstrDefaultDeviceId)
        {
            Console.WriteLine(pwstrDefaultDeviceId);
        }

        public void OnPropertyValueChanged(PCWSTR pwstrDeviceId, PROPERTYKEY key)
        {
            Console.WriteLine(pwstrDeviceId);
        }
    }
}
