using System;

using EliasGamma.Coder;
using EliasGamma.Decoder;

namespace EliasGamma
{
    class Program
    {
        static void Main(string[] args)
        {
            String text = String.Empty;

            Console.Write("Enter text: ");

            text = Console.ReadLine();

            String code = EliasGammaCoder.Code(text);

            Console.WriteLine($"Elias Gamma code: {code}");

            String decodedText = EliasGammaDecoder.Decode(code);

            Console.WriteLine($"Decoded text: {decodedText}");

            Console.ReadKey();
        }
    }
}
