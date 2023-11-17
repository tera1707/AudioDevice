using System.Data;
using System.Diagnostics;
using System.Runtime.InteropServices;

namespace MicJikkenCs
{
    internal class Program
    {
        static void Main(string[] args)
        {
            Console.WriteLine("Hello, World!");

            var pEnum = new MMDeviceEnumerator();
            var pNotifClient = new CMMNotificationClient();

            if (pEnum is IMMDeviceEnumerator p)
            {
                var HResult = p.RegisterEndpointNotificationCallback(pNotifClient);
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
    public void dummy1();
    [PreserveSig]
    public void dummy2();
    [PreserveSig]
    public void dummy3();
    [PreserveSig]
    public int RegisterEndpointNotificationCallback(IMMNotificationClient pClient);
}

//[ComImport, InterfaceType(ComInterfaceType.InterfaceIsIUnknown)]
//[Guid("7991EEC9-7E89-4D85-8390-6C703CEC60C0")]
[ComImport]
[Guid("7991EEC9-7E89-4D85-8390-6C703CEC60C0")]
[InterfaceType(ComInterfaceType.InterfaceIsIUnknown)]
internal interface IMMNotificationClient
{
    //[PreserveSig]
    //public int OnDeviceStateChanged([MarshalAs(UnmanagedType.LPWStr)] string pwstrDeviceId, ulong dwNewState);
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
        Debug.WriteLine(pwstrDeviceId);
        Debug.WriteLine("");
        return 0;
    }

    public int OnDeviceAdded(string pwstrDeviceId)
    {
        //Debug.WriteLine(pwstrDeviceId);
        return 0;
    }

    public int OnDeviceRemoved(string pwstrDeviceId)
    {
        //Debug.WriteLine(pwstrDeviceId);
        return 0;
    }

    public int OnDefaultDeviceChanged(EDataFlow flow, ERole role, string pwstrDefaultDeviceId)
    {
        //Debug.WriteLine(pwstrDefaultDeviceId);
        return 0;
    }

    public int OnPropertyValueChanged(string pwstrDeviceId, ref PROPERTYKEY key)
    {
        //Debug.WriteLine(pwstrDeviceId);
        return 0;
    }
}