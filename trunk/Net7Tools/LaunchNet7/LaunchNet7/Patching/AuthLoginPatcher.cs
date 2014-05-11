using System;
using System.Collections.Generic;
using System.Text;
using System.IO;

namespace LaunchNet7.Patching
{
    public class AuthLoginPatcher
    {
        static readonly byte Https = 0xc0;
        static readonly byte Http = 0x40;

        public static AuthPatcherInfo ReadInformation(string fileName)
        {
            if (File.Exists(fileName) == false) throw new FileNotFoundException();

            AuthPatcherInfo info = new AuthPatcherInfo();
            using (FileStream fs = File.OpenRead(fileName))
            {
                fs.Seek(0x8328, SeekOrigin.Begin);
                byte current = (byte)fs.ReadByte();
                if (current == Https)
                {
                    info.UseHttps = true;
                }
                else if (current == Http)
                {
                    info.UseHttps = false;
                }
                else
                {
                    throw new InvalidDataException();
                }

                fs.Seek(0x82AD, SeekOrigin.Begin);
                byte[] port = new byte[2];
                fs.Read(port, 0, 2);
                info.Port = BitConverter.ToUInt16(port, 0);

                fs.Seek(0x8292, SeekOrigin.Begin);
                byte[] timeout = new byte[2];
                fs.Read(timeout, 0, 2);
                info.TimeOut = BitConverter.ToUInt16(timeout, 0);
            }

            return info;
        }

        public static void WriteInformation(string fileName, AuthPatcherInfo infos)
        {
            if (infos == null) throw new ArgumentNullException("infos");
            if (File.Exists(fileName) == false) throw new FileNotFoundException();

            byte[] buffer;
            using (FileStream fs = File.OpenWrite(fileName))
            {
                fs.Seek(0x8328, SeekOrigin.Begin);
                fs.WriteByte(infos.UseHttps ? Https : Http);

                fs.Seek(0x82AD, SeekOrigin.Begin);
                buffer = BitConverter.GetBytes(infos.Port);
                fs.Write(buffer, 0, 2);

                fs.Seek(0x8292, SeekOrigin.Begin);
                buffer = BitConverter.GetBytes(infos.TimeOut);
                fs.Write(buffer, 0, 2);
            }
        }
    }
}
