package guet.pyl.gobang;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Point;
import android.graphics.PorterDuff.Mode;
import android.graphics.Rect;
import android.graphics.drawable.BitmapDrawable;
import android.util.AttributeSet;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

public class MyView extends SurfaceView implements SurfaceHolder.Callback {
	private SurfaceHolder holder = null;
	private Chessboard m_chessboard = null;
	
	private Point m_leftTop; 	//开始绘制的点
	private int m_gridSize;
	private int m_pieceSize;
	
	private Bitmap m_bit_black, m_bit_white;
	private Rect m_rectBit;	//棋子的绘制的rect
	
	public MyView(Context context, AttributeSet attr) {
		super(context, attr);
		// TODO Auto-generated constructor stub
		holder = getHolder();
		holder.addCallback(this);
		this.m_chessboard = null;
	}
	@Override
	public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {
	}

	@Override
	public void surfaceCreated(SurfaceHolder holder) {
		new Thread(new MyLoop()).start();
	}
	public void LoadChessboard(Chessboard chessboard, int width)
	{
		if(chessboard == null)
			return;
		this.m_chessboard = chessboard;
		
		m_gridSize = width / m_chessboard.GetBoardSize();
		m_pieceSize = (int)(m_gridSize * 1.0);	//棋子与格子的比例
		
		m_leftTop = new Point();
		m_leftTop.x = m_gridSize/2;
		m_leftTop.y = m_gridSize/2;//m_screenHeight - m_gridSize * m_chessboard.GetBoardSize() - m_gridSize;
		
		m_bit_black = ((BitmapDrawable)getResources().getDrawable(R.drawable.black16)).getBitmap();
		m_bit_white = ((BitmapDrawable)getResources().getDrawable(R.drawable.white16)).getBitmap();
		m_rectBit = new Rect(0, 0, m_bit_black.getWidth(), m_bit_black.getHeight());
	}

	@Override
	public void surfaceDestroyed(SurfaceHolder holder) {
	}

	private void drawChessboardLine(Canvas canvas, Paint paint)
	{
		int sx, sy, ex, ey;
		sx = m_leftTop.x;
		ex = sx + (m_chessboard.GetBoardSize() - 1) * m_gridSize;
		//绘制第一条和最后一条横线
		paint.setStrokeWidth(1.5f);
		ey = sy = m_leftTop.y;
		canvas.drawLine(sx, sy, ex, ey, paint);
		ey = sy = m_leftTop.y + (m_chessboard.GetBoardSize() - 1) * m_gridSize;
		canvas.drawLine(sx, sy, ex, ey, paint);
		
		paint.setStrokeWidth(1);
		for(int i = 1; i < m_chessboard.GetBoardSize() - 1; i++)//画横线
		{
			ey = sy = m_leftTop.y + i * m_gridSize;
			canvas.drawLine(sx, sy, ex, ey, paint);
		}
		
		sy = m_leftTop.y;
		ey = sy + (m_chessboard.GetBoardSize() - 1) * m_gridSize;
		paint.setStrokeWidth(1.5f);
		sx = ex = m_leftTop.x; 
		canvas.drawLine(sx, sy, ex, ey, paint);
		sx = ex = m_leftTop.x + (m_chessboard.GetBoardSize() - 1) * m_gridSize; 
		canvas.drawLine(sx, sy, ex, ey, paint);

		paint.setStrokeWidth(1);
		for(int i = 1; i < m_chessboard.GetBoardSize() - 1; i++)//画竖线
		{
			sx = ex = m_leftTop.x + i * m_gridSize;
			canvas.drawLine(sx, sy, ex, ey, paint);
		}
	}
	private void drawChessboardMark(Canvas canvas, Paint paint)
	{
		float radius = 2.3f;
		int markIndex = 3;
		float cx, cy;
		cx = m_leftTop.x + markIndex * m_gridSize + 0.5f;
		cy = m_leftTop.y + markIndex * m_gridSize + 0.5f;
		canvas.drawCircle(cx, cy, radius, paint);
		
		cx = m_leftTop.x + (m_chessboard.GetBoardSize() - 1 - markIndex) * m_gridSize + 0.5f;
		cy = m_leftTop.y + markIndex * m_gridSize + 0.5f;
		canvas.drawCircle(cx, cy, radius, paint);
		
		cx = m_leftTop.x + markIndex * m_gridSize + 0.5f;
		cy = m_leftTop.y + (m_chessboard.GetBoardSize() - 1 - markIndex) * m_gridSize + 0.5f;
		canvas.drawCircle(cx, cy, radius, paint);
		
		cx = m_leftTop.x + (m_chessboard.GetBoardSize() - 1 - markIndex) * m_gridSize + 0.5f;
		cy = m_leftTop.y + (m_chessboard.GetBoardSize() - 1 - markIndex) * m_gridSize + 0.5f;
		canvas.drawCircle(cx, cy, radius, paint);
	}
	private void drawChessboardPieces(Canvas canvas, Paint paint)
	{
		int l, t, b, r;
		for(int i = 0; i < m_chessboard.GetBoardSize(); i++)
		{
			t = m_leftTop.y + i * m_gridSize - m_pieceSize/2 + 2;	//2表示绘制偏移量
			b = t + m_pieceSize;
			for(int j = 0; j < m_chessboard.GetBoardSize(); j++)
			{
				l = m_leftTop.x + j * m_gridSize - m_pieceSize/2 + 2;
				r = l + m_pieceSize;
				Rect rectCB = new Rect(l,t,r,b);
				if(m_chessboard.GetGrid(i, j) == 2)//black
				{
					canvas.drawBitmap(m_bit_black, m_rectBit, rectCB, paint);
				}
				else if(m_chessboard.GetGrid(i, j) == 3)//white
				{
					canvas.drawBitmap(m_bit_white, m_rectBit, rectCB, paint);
				}
			}
			
		}
	}
	private void drawChessboardChengWu(Canvas canvas, Paint paint)
	{
		int[] a = m_chessboard.GetChengWu();
		if(a == null)
			return;
		int oldColor = paint.getColor();
		paint.setColor(Color.RED);
		paint.setAntiAlias(true);
		float cx, cy;
		for(int i = 0; i < a.length; i+=2)
		{
			cy = m_leftTop.y + a[i] * m_gridSize + 0.5f;
			cx = m_leftTop.x + a[i + 1] * m_gridSize + 0.5f;
			canvas.drawCircle(cx, cy, 3, paint);				
		}
		paint.setColor(oldColor);
		paint.setAntiAlias(false);
	}
	private void drawChessboardLastMark(Canvas canvas, Paint paint)
	{
		int a[] = m_chessboard.GetLastStep();
		if(a == null)
			return;
		int oldColor = paint.getColor();
		paint.setColor(Color.RED);
		paint.setAntiAlias(true);
		float cx, cy;
		cy = m_leftTop.y + a[0] * m_gridSize + 0.5f;
		cx = m_leftTop.x + a[1] * m_gridSize + 0.5f;
		canvas.drawCircle(cx, cy, 2, paint);	
		paint.setColor(oldColor);
		paint.setAntiAlias(false);
	}
	
	public void doDraw(Canvas canvas) {
		// TODO Auto-generated method stub
		super.draw(canvas);
		if(this.m_chessboard == null)
			return;

		canvas.drawColor(Color.TRANSPARENT, Mode.CLEAR);
		Paint paint = new Paint();
		paint.setColor(Color.BLACK);
		
		drawChessboardLine(canvas, paint);
		drawChessboardMark(canvas, paint);
		drawChessboardPieces(canvas, paint);
		drawChessboardChengWu(canvas, paint);
		drawChessboardLastMark(canvas, paint);
	}
	
	public boolean AddPiece(int clickX, int clickY)
	{
		int x = clickX - this.getLeft();	//转化为view自己的坐标
		int y = clickY - this.getTop();	
				
		int row = y - m_leftTop.y + m_gridSize/2;
		int col = x - m_leftTop.x + m_gridSize/2;
		
		row /= m_gridSize;
		col /= m_gridSize;
		if(m_chessboard.AddPiece(row, col) > 0)
			return true;
		return false;
	}
	public boolean AddPiece2(int row, int col)
	{
		if(m_chessboard.AddPiece(row, col) > 0)
			return true;
		return false;
	}
	
	class MyLoop implements Runnable {
		@Override
		public void run() {
			// TODO Auto-generated method stub
			while (true) {
				try {
					Canvas c = holder.lockCanvas();
					doDraw(c);
					holder.unlockCanvasAndPost(c);
					Thread.sleep(100);
				} catch (Exception e) {
				}
			}
		}

	}
}