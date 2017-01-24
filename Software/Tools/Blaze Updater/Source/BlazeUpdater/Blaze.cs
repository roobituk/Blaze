using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace BlazeUpdater
{
    public class Blaze
    {
        private ISerialProvider _provider;

        private IChecksum<ushort> _checksum;

        private AutoResetEvent _replyReceived;

        private ByteList _dataBuffer;

        private int _startTime;

        private int _endTime;

        public int BaudRate
        {
            get
            {
                return _provider.BaudRate;
            }
            set
            {
                _provider.BaudRate = value;
            }
        }

        public ByteList DataBuffer
        {
            get
            {
                return _dataBuffer;
            }
        }

        public string PortName
        {
            get
            {
                return _provider.PortName;
            }
            set
            {
                _provider.PortName = value;
            }
        }

        public int ResponseTime
        {
            get
            {
                return _endTime - _startTime;
            }
        }


        public bool IsOpen
        {
            get
            {
                return _provider.IsOpen;
            }
        }

        public int Timeout
        {
            get
            {
                return _provider.Timeout;
            }
            set
            {
                _provider.Timeout = value;
            }
        }

        public Blaze()
        {
            _provider = new SerialProvider();
            _provider.DataReceived += new EventHandler(OnDataReceived);
            _replyReceived = new AutoResetEvent(false);
            _provider.PortName = "COM1";
            _provider.BaudRate = 115200;
            _checksum = new CCITT();
            _dataBuffer = new ByteList();
        }

        public void Close()
        {
            _provider.Close();
        }

        private void OnDataReceived(object sender, EventArgs e)
        {
            if (_provider.ReadBuffer.Count > 0)
            {
                string data = Encoding.ASCII.GetString(_provider.ReadBuffer.ToArray());

                if (data.Contains("\r\n"))
                {
                    if (data.Contains("OK"))
                    {
                        _replyReceived.Set();
                    }
                }
            }

            /*
            // Find the sync character index
            int sync = _provider.ReadBuffer.FindIndex(x => x == 35);

            // We have received a sync character
            if (sync >= 0)
            {
                // Check to see if we have enough bytes to read the data count
                if (_provider.ReadBuffer.Count >= (sync + 3))
                {
                    // Get the index of the data count
                    int dataIndex = _provider.ReadBuffer[sync + 2];
                    // Get the number of expected data bytes
                    int dataCount = _provider.ReadBuffer[dataIndex];

                    // Check if we've got enough data for a full message (header + csum)
                    if (_provider.ReadBuffer.Count > (dataCount + 5))
                    {
                        _dataBuffer.AddRange(_provider.ReadBuffer.GetRange(sync, dataCount + 5));
                        _replyReceived.Set();
                    }
                }
            }*/

        }

        public void Open()
        {
            try
            {
                _provider.Open();
            }
            catch
            {
                throw;
            }
        }

        internal bool Write(BlazeCommand command, params byte[] data)
        {
            ByteList byteList = new ByteList();
            ushort length = 0;

            if (data != null)
            {
                length = (ushort)data.Length;
            }

            // Add a '$' sync character
            byteList.Add(36);
            byteList.Add((byte)command);
            byteList.AddUInt16(length);

            if (length > 0)
            {
                byteList.AddRange(data);
            }

            ushort csum = 0;

            if (_checksum != null)
            {
                // Don't count the 
                csum = _checksum.Calculate(byteList, 0, byteList.Count);
            }

            byteList.Add((byte)(csum >> 8));
            byteList.Add((byte)csum);

            byteList.TrimExcess();

            _replyReceived.Reset();

            _startTime = Environment.TickCount;

            if (_dataBuffer != null)
            {
                _dataBuffer.Clear();
            }

            _provider.Write(byteList);

            bool flag = _replyReceived.WaitOne(_provider.Timeout);

            _endTime = Environment.TickCount;

            return flag;
        }

        public event EventHandler ReplyReceived;

    }
}
