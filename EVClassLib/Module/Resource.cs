using System;
using System.Linq;

namespace EVClassLib
{
    public enum ResType
    {
        File = 0,
        Audio = 1,
        Image = 2,
        Other = 3
    }

    [Serializable]
    public class Resource
    {
        /// <summary>
        /// 资源名称，名称不能重复 不带后缀
        /// </summary>
        public string Name { get; set; }

        /// <summary>
        /// 资源类型
        /// </summary>
        public ResType Type { set; get; }

        /// <summary>
        /// 资源相对于模块的路径信息，带后缀名
        /// </summary>
        public string Path { get; set; }

        public Resource()
        {

        }

        public Resource(string name, string path, ResType type)
        {
            this.Name = name;
            this.Path = path;
            this.Type = type;
        }

        public Data ToData()
        {
            string value = @"0:/Resources/" + System.IO.Path.GetFileName(Path);
            return new Data(Name, value);
        }

        /// <summary>
        /// 根据后缀获取类型
        /// </summary>
        /// <param name="ext"></param>
        /// <returns></returns>
        public ResType GetTypeByExt(string ext)
        {
            string[] imageExt = new string[] { ".tif", ".png", ".jpg", ".bmp" };
            string[] audioExt = new string[] { ".mp3", ".wav" };
            if (imageExt.Contains(ext))
                return ResType.Image;
            else if (audioExt.Contains(ext))
                return ResType.Audio;
            else
                return ResType.Other;
        }
    }
}
