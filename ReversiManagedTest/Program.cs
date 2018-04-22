using System;
using Freiyer.Games;

namespace ReversiManagedTest
{
    class Program
    {
        static void Main(string[] args)
        {
            char i, j;
            Console.WriteLine();

            Reversi reversi = new Reversi();
            while(((int)reversi.GetState() & 1) != 0)
            {
                Console.Write(reversi);
                while (Char.IsWhiteSpace(i = (char)Console.Read())) ;
                while (Char.IsWhiteSpace(j = (char)Console.Read())) ;
                Console.Clear();
                if (Char.IsLower(i))
                    i -= ' ';
                if (Char.IsLower(j))
                    j -= ' ';
                if (i == 'Z' && j == 'Z')
                {
                    if (!reversi.Undo())
                        Console.WriteLine("Cannot undo.");
                    else
                        Console.WriteLine("Undid.");
                }
                else if (i == 'Y' && j == 'Y')
                {
                    if (!reversi.Undo())
                        Console.WriteLine("Cannot redo.");
                    else
                        Console.WriteLine("Redid.");
                }
                else if (i == 'S' && j == 'S')
                {
                    if (!reversi.SkipTurn())
                        Console.WriteLine("Cannot skip.");
                    else
                        Console.WriteLine("Skipped.");
                }
                else if (!Char.IsDigit(i) || !Char.IsLetter(j))
                {
                    Console.WriteLine("Invalid command.");
                }
                else
                {
                    if (!reversi.PlacePiece(i - '1', j - 'A'))
                        Console.WriteLine("Cannot place.");
                    else
                    {
                        if (reversi.GetState() == ReversiState.NoAvailableSpaces)
                            Console.WriteLine("No available spaces.");
                        else
                            Console.WriteLine();
                    }
                }
            }
        }
    }
}
