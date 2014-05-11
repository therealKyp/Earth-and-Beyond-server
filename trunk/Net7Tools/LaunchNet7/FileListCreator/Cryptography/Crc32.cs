using System;
using System.Collections.Generic;
using System.Text;

namespace FileListCreator.Cryptography
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
    }
}
