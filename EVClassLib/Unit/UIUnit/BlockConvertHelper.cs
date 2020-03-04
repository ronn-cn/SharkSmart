using EVTechnology.Common.Controls;
using System;
using System.Reflection;

namespace EVClassLib
{
    /// <summary>
    /// 视图转换
    /// </summary>
    public static class BlockConvertHelper
    {
        public static SControl ConvertClass(UIBlock block)
        {
            SControl ctl;
            switch (block.Type)
            {
                case BlockType.BUTTON:
                    ctl = new SButton();
                    break;
                case BlockType.EDIT:
                    ctl = new SEdit();
                    break;
                case BlockType.TEXT:
                    ctl = new SText();
                    break;
                case BlockType.MULTIEDIT:
                    ctl = new SMultiEdit();
                    break;
                case BlockType.RADIO:
                    ctl = new SRadio();
                    break;
                case BlockType.CHECKBOX:
                    ctl = new SCheckBox();
                    break;
                case BlockType.IMAGE:
                    ctl = new SImage();
                    break;
                case BlockType.HEADER:
                    ctl = new SHeader();
                    break;
                case BlockType.SCROLLBAR:
                    ctl = new SScrollBar();
                    break;
                case BlockType.SLIDER:
                    ctl = new SSlider();
                    break;
                case BlockType.WINDOW:
                    ctl = new SWindow();
                    break;
                case BlockType.ICONVIEW:
                    ctl = new SIconView();
                    break;
                case BlockType.RECT:
                    ctl = new SRect();
                    break;
                case BlockType.MULTIPAGE:
                    ctl = new SMultiPage();
                    break;
                case BlockType.PROGBAR:
                    ctl = new SProgbar();
                    break;
                default:
                    return null;
            }
            AutoMapping<UIBlock, SControl>(block, ctl);
            return ctl;
        }

        /// <summary>
        /// 不同类型对象同名属性赋值
        /// </summary>
        /// <typeparam name="S">源类型</typeparam>
        /// <typeparam name="T">目标类型</typeparam>
        /// <param name="s">源对象</param>
        /// <param name="t">目标对象</param>
        public static void AutoMapping<S, T>(S s, T t)
        {
            PropertyInfo[] pps = s.GetType().GetProperties();
            Type target = t.GetType();

            foreach (var pp in pps)
            {
                try
                {
                    if (pp.Name == "Type") continue;
                    if (pp.Name == "Parent") continue;
                    PropertyInfo targetPP = target.GetProperty(pp.Name);
                    object value = pp.GetValue(s, null);

                    if (targetPP != null && value != null)
                    {
                        targetPP.SetValue(t, value, null);
                    }
                }
                catch (Exception ex)
                {
                    throw ex;
                }
            }
        }
    }
}
