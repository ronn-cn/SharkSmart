using EVTechnology.Helper;
using System;
using System.IO;

namespace EVClassLib
{
    [Serializable]
    public class Module : IFile
    {
        public Guid ID;

        /// <summary>
        /// 模块名称
        /// </summary>
        public string Name { set; get; }

        /// <summary>
        /// 父级场景Guid
        /// </summary>
        public Guid Parent { set; get; }

        /// <summary>
        /// scene/mod/mod.mt 具体到文件
        /// </summary>
        public string WorkPath { set; get; }

        public void Read()
        {

        }

        public virtual void Save()
        {
            FileHelper.CreateSerializeFile(WorkPath, this, FileType.BinaryFile);
        }

        public void Delete()
        {
            FileHelper.CmdDeleteFolder(Path.GetDirectoryName(WorkPath));
        }
    }
}
