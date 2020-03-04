using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SharkSmart
{
    public static class CommandHelper
    {
        public static List<byte> Header = new List<byte>() { 0xAB, 0xBA };
        public static byte Version = 0;
        public static SharkCommand DataToCommand(List<byte> data)
        {
            //读取的长度数据
            int readCmdLength = data[2] * 256 + data[3] + 4;
            if (readCmdLength > data.Count)
            {
                //如果读取的长度大于实际传过来的数据长度，则为数据错误，返回Null
                return null;
            }
            //移除接收到的多余数据字节
            data.RemoveRange(readCmdLength, data.Count - readCmdLength);
            SharkCommand command = new SharkCommand(data[4]);
            int index = 6;
            //循环添加key对象
            while (data.Count > index && data[index] != 0)
            {
                byte key = data[index];
                int len = data[index + 1] * 256 + data[index + 2];
                List<byte> value;
                try
                {
                    value = data.GetRange(index + 3, len);
                }
                catch (Exception e)
                {
                    System.Diagnostics.Debug.WriteLine(e.ToString());
                    return command;
                }

                SharkKey keyObj = new SharkKey(key, value);
                command.Keys.Add(keyObj);
                index += keyObj.Length;
            }
            return command;
        }

        public static List<byte> CommandToData(SharkCommand command)
        {
            int datalen = 2;
            List<byte> keyData = new List<byte>();
            foreach (SharkKey item in command.Keys)
            {
                datalen += item.Length;
                keyData.Add(item.Key);
                if (item.Value != null)
                {
                    keyData.Add((byte)(item.Value.Count >> 8));
                    keyData.Add((byte)(item.Value.Count));
                    keyData.AddRange(item.Value);
                }
            }
            List<byte> data = new List<byte>();
            data.AddRange(Header);
            data.Add((byte)(datalen >> 8));
            data.Add((byte)(datalen));
            data.Add(command.Id);
            data.Add(Version);
            data.AddRange(keyData);
            //补齐64字节
            for (int l = 0; l < 64 - (datalen + 4) % 64; l++)
            {
                data.Add(0);
            }
            return data;
        }

        /// <summary>
        /// 恢复模式命令
        /// </summary>
        /// <returns></returns>
        public static byte[] Recovery()
        {
            SharkCommand cmd = new SharkCommand(0x03);
            cmd.Keys.Add(new SharkKey(0x02, new List<byte>()));
            return CommandToData(cmd).ToArray();
        }

        /// <summary>
        /// 重启命令
        /// </summary>
        /// <returns></returns>
        public static byte[] Restart()
        {
            SharkCommand cmd = new SharkCommand(0x03);
            cmd.Keys.Add(new SharkKey(0x01, new List<byte>()));
            return CommandToData(cmd).ToArray();
        }

        public static byte[] StartWhileCpu()
        {
            SharkCommand cmd = new SharkCommand(0x04);
            cmd.Keys.Add(new SharkKey(0x09, new List<byte>()));
            return CommandToData(cmd).ToArray();
        }

        public static byte[] EndWhileCpu()
        {
            SharkCommand cmd = new SharkCommand(0x04);
            cmd.Keys.Add(new SharkKey(0x0a, new List<byte>()));
            return CommandToData(cmd).ToArray();
        }

        public static byte[] ClearVersion()
        {
            SharkCommand cmd = new SharkCommand(0x03);
            cmd.Keys.Add(new SharkKey(0x06, new List<byte>()));
            return CommandToData(cmd).ToArray();
        }
    }

    public class SharkCommand
    {
        public byte Id { get; set; }

        public int Length
        {
            get
            {
                int len = 2;
                foreach (SharkKey key in Keys)
                {
                    len += key.Length;
                }
                return len;
            }
        }

        public List<SharkKey> Keys = new List<SharkKey>();


        public SharkCommand()
        {

        }

        public SharkCommand(byte id)
        {
            this.Id = id;
        }
    }

    /// <summary>
    /// shark一条命令中的关键词对象
    /// </summary>
    public class SharkKey
    {
        private byte key;
        public byte Key
        {
            get { return key; }
            set
            {
                if (value != key)
                {
                    key = value;
                }
            }
        }

        public int Length
        {
            get { return Value.Count + 3; }
        }

        public List<byte> Value = new List<byte>();

        public SharkKey()
        {

        }

        public SharkKey(byte key, List<byte> data)
        {
            this.Key = key;
            this.Value = data;
        }
    }
}
