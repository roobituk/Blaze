using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace BlazeUpdater
{
    public abstract class BaseProvider : IProvider, IDisposable
    {
        protected ByteList _readBuffer;

        protected ByteList _writeBuffer;

        protected bool _abort;

        private int _timeout;

        private int _pollInterval;

        public abstract bool IsOpen
        {
            get;
        }

        public int PollInterval
        {
            get
            {
                return _pollInterval;
            }
            set
            {
                _pollInterval = value;
            }
        }

        public ByteList ReadBuffer
        {
            get
            {
                return _readBuffer;
            }
        }

        public int Timeout
        {
            get
            {
                return _timeout;
            }
            set
            {
                _timeout = value;
            }
        }

        public ByteList WriteBuffer
        {
            get
            {
                return _writeBuffer;
            }
        }

        protected BaseProvider()
        {
            _writeBuffer = new ByteList();
            _readBuffer = new ByteList();
            _timeout = 2000;
            _abort = false;
            _pollInterval = 10;
        }

        public abstract void Close();

        public abstract void Dispose();

        protected virtual void OnDataReceived(object sender, EventArgs e)
        {
            if (DataReceived != null)
            {
                DataReceived(sender, e);
            }
        }

        public abstract void Open();

        public abstract void Write(byte[] buffer);

        public void Write(ByteList buffer)
        {
            Write(buffer.ToArray());
        }

        public event EventHandler DataReceived;
    }
}
