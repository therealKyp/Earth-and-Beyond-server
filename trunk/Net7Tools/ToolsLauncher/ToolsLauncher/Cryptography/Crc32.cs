using System;
using System.Collections.Generic;
using System.Text;
using System.IO;

namespace ToolsLauncher.Cryptography
{
    public static class Crc32
    {
        public static UInt32[] CreateTable(UInt32 polynomial)
        {
            UInt32[] table = new UInt32[256];
            for (int i = 0; i < 256; i++)
            {
                UInt32 value = (UInt32)i;
                for (int j = 0; j < 8; j++)
                {
                    if ((value & 1) == 1)
                    {
                        value = (value >> 1) ^ polynomial;
                    }
                    else
                    {
                        value = (value >> 1);
                    }
                }
                table[i] = value;

            }
            return table;
        }

        public static UInt32 Process(byte[] bytes)
        {
            Crc32Processor processor = new Crc32Processor();
            processor.Process(bytes);
            return processor.Current;
        }

        public static UInt32 Process(byte[] buffer, int startIndex, int length)
        {
            Crc32Processor processor = new Crc32Processor();
            processor.Process(buffer, startIndex, length);
            return processor.Current;
        }

        public static UInt32 ProcessStream(Stream stream)
        {
            if (stream == null) throw new ArgumentOutOfRangeException("stream");
            return InternalProcessStream(stream, null, null);
        }
        public static UInt32 ProcessStream(Stream stream, int bufferSize)
        {
            if (stream == null) throw new ArgumentOutOfRangeException("stream");
            if (bufferSize <= 0) throw new ArgumentOutOfRangeException("bufferSize");
            return InternalProcessStream(stream, null, bufferSize);
        }
        public static UInt32 ProcessStream(Stream stream, long maxLength)
        {
            if (stream == null) throw new ArgumentOutOfRangeException("stream");
            if (maxLength < 0) throw new ArgumentOutOfRangeException("maxLength");
            return InternalProcessStream(stream, maxLength, null);
        }
        public static UInt32 ProcessStream(Stream stream, long maxLength, int bufferSize)
        {
            if (stream == null) throw new ArgumentOutOfRangeException("stream");
            if (bufferSize <= 0) throw new ArgumentOutOfRangeException("bufferSize");
            if (maxLength < 0) throw new ArgumentOutOfRangeException("maxLength");
            return InternalProcessStream(stream, maxLength, bufferSize);
        }
        private static UInt32 InternalProcessStream(Stream stream, long? maxLength, int? bufferSize)
        {
            Crc32Processor processor = new Crc32Processor();
            byte[] buffer = new byte[bufferSize.GetValueOrDefault(4096)];

            long totalBytesRead = 0;
            while (true)
            {
                int bytesRead = stream.Read(buffer, 0, maxLength.HasValue ? (int)Math.Min(buffer.Length, maxLength.Value - totalBytesRead) : buffer.Length);
                processor.Process(buffer, 0, bytesRead);
                if (maxLength.HasValue)
                {
                    totalBytesRead += bytesRead;
                    if (totalBytesRead >= maxLength.Value) break;
                }
                else if (bytesRead == 0)
                {
                    break;
                }
            }
            return processor.Current;
        }
    }
}
