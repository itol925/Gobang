package guet.pyl.gobang;

public class Chessboard {
	public native int StartGame();
	
	public native int AddPiece(int _row, int _col);
	public native int NextMove();
	public native int CheckGameOver();
	
	public native int GetGrid(int _row, int _col);
	public native int[] GetChengWu();
	public native int[] GetLastStep();
	public native int GetStepsCount();
	public native int GetBoardSize();
	public native void SetBoardSize(int size);
	public native void SetDiffcult(int diff);
	public native void ClearChessBoard();

	static
	{
		System.loadLibrary("Gobang");
	}
}
