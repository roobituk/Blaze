using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace BlazeUpdater
{
    public class ByteList : List<byte>
    {
        public ByteList()
        {
        }

        public void AddUInt16(ushort value)
        {
            byte[] bytes = BitConverter.GetBytes(value);
            Array.Reverse(bytes);
            base.AddRange(bytes);
        }

        public void AddUInt32(uint value)
        {
            byte[] bytes = BitConverter.GetBytes(value);
            Array.Reverse(bytes);
            base.AddRange(bytes);
        }

        public float ReadSingle(int index)
        {
            byte[] array = base.GetRange(index, 4).ToArray();
            Array.Reverse(array);
            return BitConverter.ToSingle(array, 0);
        }

        public ushort ReadUInt16(int index)
        {
            byte[] array = base.GetRange(index, 2).ToArray();
            Array.Reverse(array);
            return BitConverter.ToUInt16(array, 0);
        }

        public uint ReadUInt32(int index)
        {
            byte[] array = base.GetRange(index, 4).ToArray();
            Array.Reverse(array);
            return BitConverter.ToUInt32(array, 0);
        }
    }
}
