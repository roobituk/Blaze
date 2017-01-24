using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace BlazeUpdater
{
	public class CCITT : IChecksum<ushort>
    {
        public CCITT()
        {
        }

        public ushort Calculate(List<byte> buffer, int index, int length)
        {
            ushort item = 65535;
            for (int i = index; i < length; i++)
            {
                item = (ushort)(item ^ (ushort)(buffer[i] << 8));
                for (int j = 0; j < 8; j++)
                {
                    item = ((item & 32768) <= 0 ? (ushort)(item << 1) : (ushort)(item << 1 ^ 4129));
                }
            }
            return item;
        }
    }
}
