using System;
using System.IO.Ports;
using System.Threading;

namespace BlazeUpdater
{
    public class SerialProvider : BaseProvider, ISerialProvider, IProvider
    {
        private SerialPort _serialPort;

        private Thread _readThread;

        public int BaudRate
        {
            get
            {
                return _serialPort.BaudRate;
            }
            set
            {
                _serialPort.BaudRate = value;
            }
        }

        public override bool IsOpen
        {
            get
            {
                return _serialPort.IsOpen;
            }
        }

        public string PortName
        {
            get
            {
                return _serialPort.PortName;
            }
            set
            {
                _serialPort.PortName = value;
            }
        }

        public SerialProvider()
        {
            _serialPort = new SerialPort();
            _serialPort.BaudRate = 57600;
            _serialPort.WriteTimeout = 10000;
            _serialPort.ReadTimeout = 10000;
        }

        public override void Close()
        {
            try
            {
                DestroyReadThread();
            }
            finally
            {
                GC.ReRegisterForFinalize(_serialPort.BaseStream);
                _serialPort.Close();
            }
        }

        private void CreateReadThread()
        {
            try
            {
                _readThread = new Thread(new ThreadStart(ReadThread))
                {
                    Priority = ThreadPriority.AboveNormal,
                    Name = "SerialProvider",
                    IsBackground = true
                };
                _readThread.Start();
            }
            catch
            {
                throw;
            }
        }

        private void DestroyReadThread()
        {
            try
            {
                _abort = true;
            }
            finally
            {
                _readThread.Join();
                _readThread = null;
            }
        }

        public override void Dispose()
        {
        }

        public override void Open()
        {
            try
            {
                _abort = false;
                _serialPort.Open();
                GC.SuppressFinalize(_serialPort.BaseStream);
                CreateReadThread();
            }
            catch (Exception ex)
            {
                throw;
            }
        }

        private void ReadThread()
        {
            int bytesToRead = 0;
            while (!_abort)
            {
                try
                {
                    try
                    {
                        bytesToRead = _serialPort.BytesToRead;
                        if (bytesToRead != 0)
                        {
                            byte[] numArray = new byte[bytesToRead];
                            _serialPort.Read(numArray, 0, bytesToRead);
                            _readBuffer.AddRange(numArray);
                            OnDataReceived(this, EventArgs.Empty);
                        }
                    }
                    catch
                    {
                        _abort = true;
                    }
                }
                finally
                {
                    Thread.Sleep(base.PollInterval);
                }
            }
        }

        public override void Write(byte[] buffer)
        {
            try
            {
                _readBuffer.Clear();
                _writeBuffer.Clear();
                _serialPort.DiscardInBuffer();
                _serialPort.DiscardOutBuffer();
                _writeBuffer.AddRange(buffer);
                _serialPort.Write(buffer, 0, (int)buffer.Length);
            }
            catch (Exception ex)
            {
                throw;
            }
        }
    }
}
