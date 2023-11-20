using System.Diagnostics;
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

[ComImport, InterfaceType(ComInterfaceType.InterfaceIsIUnknown)]
[Guid("D666063F-1587-4E43-81F1-B948E807363F")]
internal interface IMMDevice
{
    [PreserveSig]
    public int dummy();
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