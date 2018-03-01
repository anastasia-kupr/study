using System;
using System.Linq;
using System.Text;

namespace EliasGamma.Coder
{
    public class EliasGammaCoder
    {
        public static String Code(String text)
        {
            StringBuilder sb = new StringBuilder();

            foreach(var symbol in text)
            {
                String code = GetCode(symbol);
                sb.Append(code);
            }

            return sb.ToString();
        }

        private static String GetCode(Char ch)
        {
            Int32 iCh = (Int32)ch;
            Int32 power2 = 0;

            // Get unary binary number power
            while (iCh >= (Int32)Math.Pow(2, power2))
            {
                power2++;
            }
            
            String zeroPrefix = new String(Enumerable.Repeat('0', power2).ToArray());

            return $"{zeroPrefix}{Convert.ToString(iCh, 2)}";
        }
    }
}
