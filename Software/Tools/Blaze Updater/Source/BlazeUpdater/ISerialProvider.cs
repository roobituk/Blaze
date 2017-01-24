using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace BlazeUpdater
{
    public interface ISerialProvider : IProvider
    {
        int BaudRate
        {
            get;
            set;
        }

        string PortName
        {
            get;
            set;
        }
    }
}
