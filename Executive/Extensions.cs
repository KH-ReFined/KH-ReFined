using System;
using System.Collections.Generic;
using System.Linq;

namespace ReFined.Executive
{
    public static class Extensions
    {
        public static int FetchIndexOf<T>(this List<T> Source, Func<T, bool> Predicate)
        {
            var _fetchElement = Source.First(Predicate);
            return Source.IndexOf(_fetchElement);
        }

        public static ulong FindValue(this byte[] Source, byte[] Value)
        {
            ulong _charSlot = (ulong)(Source.Length - Value.Length + 1);

            for (ulong i = 0; i < _charSlot; i++)
            {
                if (Source[i] != Value[0])
                    continue;

                for (ulong j = (ulong)Value.Length - 1; j >= 1; j--)
                {
                    if (Source[i + j] != Value[j])
                        break;

                    if (j == 1)
                        return i;
                }
            }
            return 0xFFFFFFFFFFFFFFFF;
        }

        public static ulong FindValue<T>(this byte[] Source, T Value)
        {
            var _pattern = (byte[])typeof(BitConverter).GetMethod("GetBytes", new[] { typeof(T) }).Invoke(null, new object[] { Value });
            ulong _charSlot = (ulong)(Source.Length - _pattern.Length + 1);

            for (ulong i = 0; i < _charSlot; i++)
            {
                if (Source[i] != _pattern[0])
                    continue;

                for (ulong j = (ulong)_pattern.Length - 1; j >= 1; j--)
                {
                    if (Source[i + j] != _pattern[j])
                        break;

                    if (j == 1)
                        return i;
                }
            }
            return 0xFFFFFFFFFFFFFFFF;
        }
    }
}
