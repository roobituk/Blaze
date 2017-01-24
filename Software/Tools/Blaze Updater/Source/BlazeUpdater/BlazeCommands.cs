using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace BlazeUpdater
{
    public static class BlazeCommands
    {
        /// <summary>
        /// 
        /// </summary>
        /// <param name="blaze"></param>
        /// <returns></returns>
        public static bool IsAlive(Blaze blaze)
        {
            // Send an alive
            bool flag = blaze.Write(BlazeCommand.IsAlive, null);

            return flag;
        }

        /// <summary>
        /// Starts a firmware update procedure
        /// </summary>
        /// <param name="blaze"></param>
        /// <param name="length"></param>
        /// <returns></returns>
        public static bool BeginFirmwareUpdate(Blaze blaze, uint length)
        {
            // Convert the length int to a byte array
            byte[] data = BitConverter.GetBytes(length);

            // Issue a begin firmware update command
            return blaze.Write(BlazeCommand.BeginFirmwareUpdate, data);

        }

        /// <summary>
        /// Sends a chunk of firmware data to Blaze
        /// </summary>
        /// <param name="blaze"></param>
        /// <param name="index"></param>
        /// <param name="chunk"></param>
        /// <returns></returns>
        public static bool WriteFirmwareChunk(Blaze blaze, ushort index, List<byte> chunk)
        {
            ByteList data = new ByteList();
            data.AddUInt16(index);
            data.AddRange(chunk);

            return blaze.Write(BlazeCommand.WriteFirmwareChunk, data.ToArray());
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="blaze"></param>
        /// <returns></returns>
        public static bool EndFirmwareUpdate(Blaze blaze)
        {
            blaze.Timeout = 0;

            return blaze.Write(BlazeCommand.EndFirmwareUpdate, null);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="blaze"></param>
        /// <returns></returns>
        public static bool TestCommand(Blaze blaze)
        {
            // Send some dummy bytes to Blaze for testing
            ByteList data = new ByteList();
            data.Add(0x11);
            data.Add(0x22);
            data.Add(0x33);
            data.Add(0x44);

            return blaze.Write(BlazeCommand.TestCommand, data.ToArray());
        }
    }
}
