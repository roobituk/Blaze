using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace BlazeUpdater
{
	public interface IChecksum<T>
    {
        T Calculate(List<byte> buffer, int index, int length);
    }
}
