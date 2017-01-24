using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace BlazeUpdater
{
    public interface IProvider
    {
        bool IsOpen
        {
            get;
        }

        int PollInterval
        {
            get;
            set;
        }

        ByteList ReadBuffer
        {
            get;
        }

        int Timeout
        {
            get;
            set;
        }

        ByteList WriteBuffer
        {
            get;
        }

        void Close();

        void Open();

        void Write(ByteList buffer);

        void Write(byte[] buffer);

        event EventHandler DataReceived;
    }
}
