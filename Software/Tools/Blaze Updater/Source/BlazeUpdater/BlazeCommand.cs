using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace BlazeUpdater
{
    public enum BlazeCommand : byte
    {
        IsAlive = 0x01,
        BeginFirmwareUpdate = 0x20,
        WriteFirmwareChunk = 0x21,
        EndFirmwareUpdate = 0x22,
        TestCommand = 0xFF
    }
}
