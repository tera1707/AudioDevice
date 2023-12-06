using System.Runtime.InteropServices;

namespace MicJikkenCs
{
    internal class Program
    {
        static void Main(string[] args)
        {
            var pEnum = new MMDeviceEnumerator() as IMMDeviceEnumerator;
            var pNotifClient = new CMMNotificationClient();

            var hr = pEnum!.RegisterEndpointNotificationCallback(pNotifClient);

            hr = pEnum!.EnumAudioEndpoints(EDataFlow.eCapture, (ulong)DEVICE_STATE.ACTIVE, out var pCollection);

            hr = pCollection!.GetCount(out var deviceCount);

            for (uint i = 0; i < deviceCount; i++)
            {
                hr = pCollection.Item(i, out var pEndpoint);

                hr = pEndpoint.OpenPropertyStore((ulong)STGM.READ, out var pProperties);

                var PKEY_Device_FriendlyName = new PROPERTYKEY(new Guid(0xa45c254e, 0xdf1c, 0x4efd, 0x80, 0x20, 0x67, 0xd1, 0x46, 0xa8, 0x50, 0xe0), 14);
                hr = pProperties.GetValue(PKEY_Device_FriendlyName, out var vName);

                var name = Marshal.PtrToStringUni(vName.pwszVal);
                Console.WriteLine(name);

                Guid IID_IAudioEndpointVolume = typeof(IAudioEndpointVolume).GUID;
                hr = pEndpoint.Activate(IID_IAudioEndpointVolume, 0, IntPtr.Zero, out var endpointVolume);
                IAudioEndpointVolume masterVol = (IAudioEndpointVolume)endpointVolume;

                hr = masterVol.GetMasterVolumeLevelScalar(out var pMasterVolumeLevel);
                hr = masterVol.SetMasterVolumeLevelScalar(0.30f, new Guid());
            }

            Console.ReadLine();
        }
    }
}

[ComImport]
[Guid("BCDE0395-E52F-467C-8E3D-C4579291692E")]
internal class MMDeviceEnumerator
{
}

[ComImport, InterfaceType(ComInterfaceType.InterfaceIsIUnknown)]
[Guid("A95664D2-9614-4F35-A746-DE8DB63617E6")]
internal interface IMMDeviceEnumerator
{
    [PreserveSig]
    public int EnumAudioEndpoints(EDataFlow dataFlow, ulong dwStateMask, out IMMDeviceCollection ppDevices);
    [PreserveSig]
    public int dummy2();
    [PreserveSig]
    public int dummy3();
    [PreserveSig]
    public int RegisterEndpointNotificationCallback(IMMNotificationClient pClient);
}

[ComImport, InterfaceType(ComInterfaceType.InterfaceIsIUnknown)]
[Guid("0BD7A1BE-7A1A-44DB-8397-CC5392387B5E")]
internal interface IMMDeviceCollection
{
    [PreserveSig]
    public int GetCount(out uint pcDevices);
    [PreserveSig]
    public int Item(uint nDevice, out IMMDevice ppDevice);
}

// 参考
// https://shikaku-sh.hatenablog.com/entry/c-charp-how-to-use-core-audio-api
[ComImport, InterfaceType(ComInterfaceType.InterfaceIsIUnknown)]
[Guid("D666063F-1587-4E43-81F1-B948E807363F")]
internal interface IMMDevice
{
    [PreserveSig]
    public int Activate(Guid iid, ulong dwClsCtx, IntPtr pActivationParams, [MarshalAs(UnmanagedType.IUnknown)] out object ppInterface);
    [PreserveSig]
    public int OpenPropertyStore(ulong stgmAccess, out IPropertyStore ppProperties);
}

[ComImport, InterfaceType(ComInterfaceType.InterfaceIsIUnknown)]
[Guid("886d8eeb-8cf2-4446-8d02-cdba1dbdcf99")]
internal interface IPropertyStore
{
    [PreserveSig]
    public int dummy1();
    [PreserveSig]
    public int dummy2();
    [PreserveSig]
    public int GetValue(PROPERTYKEY key, out PROPVARIANT prop);
}


[ComImport, InterfaceType(ComInterfaceType.InterfaceIsIUnknown)]
[Guid("7991EEC9-7E89-4D85-8390-6C703CEC60C0")]
internal interface IMMNotificationClient
{
    [PreserveSig]
    int OnDeviceStateChanged(string pwstrDeviceId, uint dwNewState);
    [PreserveSig]
    int OnDeviceAdded(string pwstrDeviceId);
    [PreserveSig]
    int OnDeviceRemoved(string pwstrDeviceId);
    [PreserveSig]
    int OnDefaultDeviceChanged(EDataFlow flow, ERole role, string pwstrDefaultDeviceId);
    [PreserveSig]
    int OnPropertyValueChanged(string pwstrDeviceId, ref PROPERTYKEY key);
}

[ComImport, InterfaceType(ComInterfaceType.InterfaceIsIUnknown)]
[Guid("657804FA-D6AD-4496-8A60-352752AF4F89")]
internal interface IAudioEndpointVolumeCallback
{
    [PreserveSig]
    public int OnNotify(ref AUDIO_VOLUME_NOTIFICATION_DATA pNotify);
}

[Guid("5CDF2C82-841E-4546-9722-0CF74078229A"), InterfaceType(ComInterfaceType.InterfaceIsIUnknown)]
public interface IAudioEndpointVolume
{
    [PreserveSig]
    int NotImpl1();

    [PreserveSig]
    int NotImpl2();

    [PreserveSig]
    int GetChannelCount(
        [Out][MarshalAs(UnmanagedType.U4)] out UInt32 channelCount);

    [PreserveSig]
    int SetMasterVolumeLevel(
        [In][MarshalAs(UnmanagedType.R4)] float level,
        [In][MarshalAs(UnmanagedType.LPStruct)] Guid eventContext);

    [PreserveSig]
    int SetMasterVolumeLevelScalar(
        [In][MarshalAs(UnmanagedType.R4)] float level,
        [In][MarshalAs(UnmanagedType.LPStruct)] Guid eventContext);

    [PreserveSig]
    int GetMasterVolumeLevel(
        //[Out][MarshalAs(UnmanagedType.R4)] out float level);
        out float level);

    [PreserveSig]
    int GetMasterVolumeLevelScalar(
        [Out][MarshalAs(UnmanagedType.R4)] out float level);

    [PreserveSig]
    int SetChannelVolumeLevel(
        [In][MarshalAs(UnmanagedType.U4)] UInt32 channelNumber,
        [In][MarshalAs(UnmanagedType.R4)] float level,
        [In][MarshalAs(UnmanagedType.LPStruct)] Guid eventContext);

    [PreserveSig]
    int SetChannelVolumeLevelScalar(
        [In][MarshalAs(UnmanagedType.U4)] UInt32 channelNumber,
        [In][MarshalAs(UnmanagedType.R4)] float level,
        [In][MarshalAs(UnmanagedType.LPStruct)] Guid eventContext);

    [PreserveSig]
    int GetChannelVolumeLevel(
        [In][MarshalAs(UnmanagedType.U4)] UInt32 channelNumber,
        [Out][MarshalAs(UnmanagedType.R4)] out float level);

    [PreserveSig]
    int GetChannelVolumeLevelScalar(
        [In][MarshalAs(UnmanagedType.U4)] UInt32 channelNumber,
        [Out][MarshalAs(UnmanagedType.R4)] out float level);

    [PreserveSig]
    int SetMute(
        [In][MarshalAs(UnmanagedType.Bool)] Boolean isMuted,
        [In][MarshalAs(UnmanagedType.LPStruct)] Guid eventContext);

    [PreserveSig]
    int GetMute(
        [Out][MarshalAs(UnmanagedType.Bool)] out Boolean isMuted);

    [PreserveSig]
    int GetVolumeStepInfo(
        [Out][MarshalAs(UnmanagedType.U4)] out UInt32 step,
        [Out][MarshalAs(UnmanagedType.U4)] out UInt32 stepCount);

    [PreserveSig]
    int VolumeStepUp(
        [In][MarshalAs(UnmanagedType.LPStruct)] Guid eventContext);

    [PreserveSig]
    int VolumeStepDown(
        [In][MarshalAs(UnmanagedType.LPStruct)] Guid eventContext);

    [PreserveSig]
    int QueryHardwareSupport(
        [Out][MarshalAs(UnmanagedType.U4)] out UInt32 hardwareSupportMask);

    [PreserveSig]
    int GetVolumeRange(
        [Out][MarshalAs(UnmanagedType.R4)] out float volumeMin,
        [Out][MarshalAs(UnmanagedType.R4)] out float volumeMax,
        [Out][MarshalAs(UnmanagedType.R4)] out float volumeStep);
}

struct AUDIO_VOLUME_NOTIFICATION_DATA
{
    Guid guidEventContext;
    int bMuted;
    float fMasterVolume;
    uint nChannels;
    float[] afChannelVolumes;
}

public enum EDataFlow
{
    eRender = 0,
    eCapture = (EDataFlow.eRender + 1),
    eAll = (EDataFlow.eCapture + 1),
    EDataFlow_enum_count = (EDataFlow.eAll + 1)
}

internal enum DEVICE_STATE
{
    ACTIVE = 1,
    DISABLED = 2,
    NOTPRESENT = 4,
    UNPLUGGED = 8,
}

internal enum STGM
{
    READ,
    WRITE,
    READWRITE,
}

public struct PROPERTYKEY
{
    public PROPERTYKEY(Guid InputId, uint InputPid)
    {
        fmtid = InputId;
        pid = InputPid;
    }

    private Guid fmtid;
    private uint pid;
}

[StructLayout(LayoutKind.Explicit)]
public struct PROPVARIANT
{
    [FieldOffset(0)]
    public ushort vt;

    [FieldOffset(2)]
    public ushort wReserved1;

    [FieldOffset(4)]
    public ushort wReserved2;

    [FieldOffset(6)]
    public ushort wReserved3;

    [FieldOffset(8)]
    public IntPtr pwszVal;
}

public enum ERole
{
    eConsole = 0,
    eMultimedia = (ERole.eConsole + 1),
    eCommunications = (ERole.eMultimedia + 1),
    ERole_enum_count = (ERole.eCommunications + 1)
}

public class CMMNotificationClient : IMMNotificationClient
{
    public int OnDeviceStateChanged(string pwstrDeviceId, uint dwNewState)
    {
        Console.WriteLine(pwstrDeviceId);
        Console.WriteLine("");
        return 0;
    }

    public int OnDeviceAdded(string pwstrDeviceId)
    {
        //Console.WriteLine(pwstrDeviceId);
        return 0;
    }

    public int OnDeviceRemoved(string pwstrDeviceId)
    {
        //Console.WriteLine(pwstrDeviceId);
        return 0;
    }

    public int OnDefaultDeviceChanged(EDataFlow flow, ERole role, string pwstrDefaultDeviceId)
    {
        //Console.WriteLine(pwstrDefaultDeviceId);
        return 0;
    }

    public int OnPropertyValueChanged(string pwstrDeviceId, ref PROPERTYKEY key)
    {
        //Console.WriteLine(pwstrDeviceId);
        return 0;
    }
}