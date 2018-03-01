using System;
using System.Text;

namespace EliasGamma.Decoder
{
    public class EliasGammaDecoder
    {
        public static String Decode(String code)
        {
            StringBuilder sb = new StringBuilder();
            Int32 pos = 0;
            Int32 codeLength = code.Length;

            while(pos < codeLength)
            {
                sb.Append(GetSymbol(code, ref pos));
            }

            return sb.ToString();
        }

        private static Char GetSymbol(String code, ref Int32 pos)
        {
            // Get the unary prefix
            Int32 dataEnd = code.IndexOf('1', pos);
            dataEnd = dataEnd != -1 ? dataEnd - pos : code.Length - pos;
            String zeroPrefix = code.Substring(pos, dataEnd);
            //Get the unary power
            Int32 power2 = zeroPrefix.Length;

            pos += power2;
            // Get the data symbol
            String data = code.Substring(pos, power2);

            pos += power2;

            return (Char)Convert.ToInt32(data, 2);
        }
    }
}
