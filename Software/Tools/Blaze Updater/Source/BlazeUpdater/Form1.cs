using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace BlazeUpdater
{
    public partial class Form1 : Form
    {
        private const int CHUNK_SIZE = 512;

        private Blaze _blaze;

        public Form1()
        {
            InitializeComponent();

            _blaze = new Blaze();

            UpdateControls();
        }

        private void ReloadPortNames()
        {
            // Enumerate the port names
            string[] ports = SerialPort.GetPortNames();

            PortNameList.BeginUpdate();
            PortNameList.Items.Clear();
            // Add the names to the list
            PortNameList.Items.AddRange(ports);
            PortNameList.EndUpdate();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            this.ReloadPortNames();
        }

        private void button2_Click(object sender, EventArgs e)
        {
            bool alive = BlazeCommands.IsAlive(_blaze);
        }

        private void UpdateControls()
        {
            if (_blaze.IsOpen)
            {
                program.Enabled = true;
                erase.Enabled = true;
                connect.Enabled = false;
            }
            else
            {
                connect.Enabled = true;
                program.Enabled = false;
                erase.Enabled = false;
            }
        }

        private void connect_Click(object sender, EventArgs e)
        {
            status.Text = "OK";

            // Set the port name
            _blaze.PortName = PortNameList.SelectedItem.ToString();
            _blaze.Timeout = 2000;
            _blaze.Open();

            UpdateControls();
        }

        private void program_Click(object sender, EventArgs e)
        {
            // Ask for a file
            OpenFileDialog dlg = new OpenFileDialog();
            dlg.Filter = "Blaze Firmware (*.bin)|*.bin";

            // Show the dialog
            if (dlg.ShowDialog() == DialogResult.OK)
            {
                // Get the file name
                string fileName = dlg.FileName;

                List<byte> dataBuffer = null;

                // Read the file into a buffer
                using (FileStream fs = new FileStream(fileName, FileMode.Open, FileAccess.Read))
                {
                    byte[] buffer = null;
                    // Resize the buffer
                    Array.Resize(ref buffer, (int)fs.Length);

                    using (BinaryReader br = new BinaryReader(fs))
                    {
                        // Read all bytes into the buffer
                        br.Read(buffer, 0, buffer.Length);
                    }

                    dataBuffer = buffer.ToList();
                }

                // Send a "begin firmware update" command
                bool success = BlazeCommands.BeginFirmwareUpdate(_blaze, (uint)(dataBuffer.Count));

                if (!success)
                {
                    MessageBox.Show("Failed to begin firmware update");
                    return;
                }

                System.Threading.Thread.Sleep(500);

                // Send the complete chunks
                int chunks = (dataBuffer.Count / CHUNK_SIZE);
                // Check to see if we need to send one last chunk
                int mod = (dataBuffer.Count % CHUNK_SIZE);

                ushort index = 0;

                progress.Value = 0;
                progress.Maximum = (mod == 0 ? chunks : chunks + 1);
    
                // Iterate through the chunks
                for (index = 0; index < chunks; index++)
                {
                    List<byte> chunk = dataBuffer.GetRange((int)(index * CHUNK_SIZE), CHUNK_SIZE);

                    success = BlazeCommands.WriteFirmwareChunk(_blaze, index, chunk);

                    if (!success)
                    {
                        MessageBox.Show("Failed to write firmware chunk");
                        return;
                    }

                    progress.Value++;

                    System.Threading.Thread.Sleep(50);
                }



                // Send the remaining bytes
                if (mod > 0)
                {
                    List<byte> chunk = dataBuffer.GetRange((int)(index * CHUNK_SIZE), mod);

                    success = BlazeCommands.WriteFirmwareChunk(_blaze, index, chunk);

                    if (!success)
                    {
                        MessageBox.Show("Failed to write last firmware chunk");
                        return;
                    }

                    progress.Value++;
                }

                System.Threading.Thread.Sleep(500);

                // Send an "end firmware update" command
                success = BlazeCommands.EndFirmwareUpdate(_blaze);

                /*
                if (!success)
                {
                    MessageBox.Show("Failed to run user application");
                    return;
                }*/

                _blaze.Close();

                UpdateControls();

            }
        }

        private void button4_Click(object sender, EventArgs e)
        {
            BlazeCommands.TestCommand(_blaze);
        }

        private void erase_Click(object sender, EventArgs e)
        {
            // Send a "begin firmware update" command
            bool success = BlazeCommands.BeginFirmwareUpdate(_blaze, 0);

            if (!success)
            {
                MessageBox.Show("Failed to begin firmware update");
                return;
            }
        }

        private void refresh_Click(object sender, EventArgs e)
        {
            this.ReloadPortNames();
        }

        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (_blaze.IsOpen)
                _blaze.Close();
        }
    }
}
