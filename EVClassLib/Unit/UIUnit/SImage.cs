using System;
using System.Drawing;

namespace EVClassLib
{
    [Serializable]
    public class SImage : SControl
    {
        public Image Image { set; get; }

        public SImage()
        {
            this.Type = UIType.IMAGE;
        }

        public override string GetCode()
        {
            string text = "";
            //if (!string.IsNullOrEmpty(this.ParentName))
            //{
            //    text += string.Format("{0} = SHARK_IMAGE_CREATE({1}, {2}, {3}, {4}, {5}, WM_CF_SHOW, 0, GUI_ID_{6}, \"Resources/{0}.bmp\");\r\n",
            //  Name, Location.X, Location.Y, Size.Width, Size.Height, ParentName, Name);
            //}
            return text;
        }
    }
}
