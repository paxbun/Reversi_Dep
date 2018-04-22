

using System;
using System.Text;
using System.Runtime.InteropServices;

namespace Freiyer.Sandbox
{
    public enum PieceState : byte
    {
        BlankPiece = 0,
        LightPiece,
        DarkPiece,
        PieceAvailable,
    }

    public enum ReversiState : byte
    {
        InProgress = 1,
        DarkWon = 2,
        LightWon = 4,
        Draw = 6,
        NoAvailableSpaces = 9,
    }

    public enum ReversiPlayer : byte
    {
        Dark = 1,
        Light
    }

    public class Reversi
    {
        [DllImport("ReversiNative.dll")]
        private extern static UIntPtr NewReversiDefault();

        [DllImport("ReversiNative.dll")]
        private extern static UIntPtr NewReversiExplicit(
            [MarshalAs(UnmanagedType.LPArray, SizeConst = 3911, ArraySubType = UnmanagedType.U1)] byte[] dst);

        [DllImport("ReversiNative.dll")]
        private extern static UIntPtr NewReversiCopy(UIntPtr other);

        [DllImport("ReversiNative.dll")]
        private extern static void DeleteReversi(UIntPtr instnace);

        [DllImport("ReversiNative.dll")]
        private extern static ReversiState GetState(UIntPtr instance);

        [DllImport("ReversiNative.dll")]
        private extern static void PrintState(UIntPtr instance, [MarshalAs(UnmanagedType.LPStr, SizeConst = 253)]StringBuilder result, int resultLen);

        [DllImport("ReversiNative.dll")]
        private extern static void CopyState(UIntPtr instance,
            [MarshalAs(UnmanagedType.LPArray, SizeConst = 64, ArraySubType = UnmanagedType.U1)] PieceState[] dst);

        [DllImport("ReversiNative.dll")]
        private extern static ReversiPlayer GetCurrentPlayer(UIntPtr instance);

        [DllImport("ReversiNative.dll")]
        private extern static int GetCurrentTurn(UIntPtr instance);

        [DllImport("ReversiNative.dll")]
        private extern static int GetBlankCount(UIntPtr instance);

        [DllImport("ReversiNative.dll")]
        private extern static int GetLightCount(UIntPtr instance);

        [DllImport("ReversiNative.dll")]
        private extern static int GetDarkCount(UIntPtr instance);

        [DllImport("ReversiNative.dll")]
        private extern static PieceState GetPieceAt(UIntPtr instance, int pi, int pj);

        [DllImport("ReversiNative.dll")]
        private extern static bool SaveState(UIntPtr instance,
            [MarshalAs(UnmanagedType.LPArray, SizeConst = 3911, ArraySubType = UnmanagedType.U1)] byte[] dst);

        [DllImport("ReversiNative.dll")]
        private extern static bool LoadState(UIntPtr instance,
            [MarshalAs(UnmanagedType.LPArray, SizeConst = 3911, ArraySubType = UnmanagedType.U1)] byte[] src);

        [DllImport("ReversiNative.dll")]
        private extern static bool Undo(UIntPtr instance);

        [DllImport("ReversiNative.dll")]
        private extern static bool IsUndoPossible(UIntPtr instance);

        [DllImport("ReversiNative.dll")]
        private extern static bool Redo(UIntPtr instance);

        [DllImport("ReversiNative.dll")]
        private extern static bool IsRedoPossible(UIntPtr instance);

        [DllImport("ReversiNative.dll")]
        private extern static bool PlacePiece(UIntPtr instnace, int pi, int pj);

        [DllImport("ReversiNative.dll")]
        private extern static bool SkipTurn(UIntPtr instance);

        private UIntPtr _CurrentInstance;

        public Reversi()
        {
            _CurrentInstance = NewReversiDefault();
        }

        public Reversi(byte[] src)
        {
            _CurrentInstance = NewReversiExplicit(src);
        }

        public Reversi(Reversi other)
        {
            _CurrentInstance = NewReversiCopy(other._CurrentInstance);
        }

        ~Reversi()
        {
            DeleteReversi(_CurrentInstance);
        }

        public ReversiState GetState()
        {
            return GetState(_CurrentInstance);
        }

        public void CopyState(PieceState[,] pieceState)
        {
            if (pieceState == null)
            {
                throw new ArgumentNullException(nameof(pieceState));
            }
            PieceState[] tmpState = new PieceState[64];
            CopyState(_CurrentInstance, tmpState);

            for(int i = 0; i < 8; i++)
                for (int j = 0; j < 8; j++)
                    pieceState[i, j] = tmpState[i * 8 + j];
        }

        public ReversiPlayer GetCurrentPlayer()
        {
            return GetCurrentPlayer(_CurrentInstance);
        }

        public int GetCurrentTurn()
        {
            return GetCurrentTurn(_CurrentInstance);
        }

        public int GetBlankCount()
        {
            return GetBlankCount(_CurrentInstance);
        }

        public int GetLightCount()
        {
            return GetLightCount(_CurrentInstance);
        }

        public int GetDarkCount()
        {
            return GetDarkCount(_CurrentInstance);
        }

        public PieceState GetPieceAt(int pi, int pj)
        {
            return GetPieceAt(_CurrentInstance, pi, pj);
        }

        public bool SaveState(byte[] dst)
        {
            return SaveState(_CurrentInstance, dst);
        }

        public bool LoadState(byte[] src)
        {
            return LoadState(_CurrentInstance, src);
        }

        public bool Undo()
        {
            return Undo(_CurrentInstance);
        }

        public bool IsUndoPossible()
        {
            return IsUndoPossible(_CurrentInstance);
        }

        public bool Redo()
        {
            return Redo(_CurrentInstance);
        }

        public bool IsRedoPossible()
        {
            return IsRedoPossible(_CurrentInstance);
        }

        public bool PlacePiece(int pi, int pj)
        {
            return PlacePiece(_CurrentInstance, pi, pj);
        }
        
        public bool SkipTurn()
        {
            return SkipTurn(_CurrentInstance);
        }

        public override string ToString()
        {
            StringBuilder result = new StringBuilder(253);
            PrintState(_CurrentInstance, result, result.Capacity);
            return result.ToString();
        }
    }
}
