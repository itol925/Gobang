package guet.pyl.gobang;

import android.os.Bundle;
import android.app.Activity;
import android.app.AlertDialog;
import android.content.DialogInterface;
import android.content.Intent;
import android.graphics.PixelFormat;
import android.util.DisplayMetrics;
import android.view.Menu;
import android.view.MotionEvent;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.ImageButton;
import android.widget.RelativeLayout;
import android.widget.TextView;

public class MainActivity extends Activity {
	private MyView myView = null;
	private Chessboard myChessboard = null;
	private int m_model;
	private int m_difficult;
	private int m_color;
	private int m_boardsize;
	private boolean m_gameStart;
	private boolean m_wait;
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);

		Bundle bun = this.getIntent().getExtras();
		m_model = bun.getInt("model");
		m_difficult = bun.getInt("difficult");
		m_color = bun.getInt("color");
		m_boardsize = bun.getInt("chessboard");
		myChessboard = new Chessboard();
		myChessboard.SetBoardSize(m_boardsize);
		
		setContentView(R.layout.activity_main);
		
		myView = (MyView)findViewById(R.id.mv_chessboard);
		myView.setZOrderOnTop(true);
		myView.getHolder().setFormat(PixelFormat.TRANSLUCENT);
		
		DisplayMetrics dm = new DisplayMetrics();
		this.getWindowManager().getDefaultDisplay().getMetrics(dm);
		
		RelativeLayout.LayoutParams rl = new RelativeLayout.LayoutParams(dm.widthPixels, dm.widthPixels);
		rl.addRule(RelativeLayout.CENTER_IN_PARENT);
		myView.setLayoutParams(rl);
		
		myView.LoadChessboard(myChessboard, dm.widthPixels);
		

		bindButtonClick();
		StartStartGame();
	}
	private void AIFirstStep()
	{
		m_wait = true;
		if(myChessboard.NextMove() == 1)
			m_wait = false;
		else{
			PostiveMsg("AI不玩了！","倒霉！","确定");
			OnGameOver("", 0);
		}
	}
	private void StartStartGame()
	{
		myChessboard.ClearChessBoard();
		m_gameStart = true;
		TextView black = (TextView)findViewById(R.id.playerBlackName);
		TextView white = (TextView)findViewById(R.id.playerWhiteName);
		if(m_model == 0)//AI模式
		{
			if(m_difficult != 10){
				myChessboard.SetDiffcult(m_difficult);
			}
			if(m_color == 0){	//用户选黑
				black.setText("黑方：玩家");
				white.setText("白方：AI");
				
				m_wait = false;
			}
			else{
				white.setText("白方：玩家");
				black.setText("黑方：AI");
				
				AIFirstStep();
			}		
		}
		else		//人人模式
		{}
	}
	private void BacktoMenu()
	{
		myChessboard.ClearChessBoard();
		Intent intent = new Intent();
		intent.setClass(MainActivity.this, StartActivity.class);
		startActivity(intent);
		MainActivity.this.finish();
	}
	
	private void bindButtonClick()
	{
		ImageButton btn = (ImageButton)findViewById(R.id.btn_ReStart);
		btn.setOnClickListener(new OnClickListener(){
			@Override
			public void onClick(View arg0) {
				if(m_gameStart == true)
				{
					new AlertDialog.Builder(MainActivity.this).setTitle("提示")
					.setMessage("战斗已经打响，确定要重新开局吗？").setPositiveButton("确定", new DialogInterface.OnClickListener() {
						@Override
						public void onClick(DialogInterface arg0, int arg1){
							StartStartGame();
						}
					}).setNegativeButton("取消",  new DialogInterface.OnClickListener(){
						@Override
						public void onClick(DialogInterface arg0, int arg1) {}
					}).show();
				}
				else
				{
					StartStartGame();
				}
			}
		});	

		ImageButton btn2 = (ImageButton)findViewById(R.id.btn_backToMenu);
		btn2.setOnClickListener(new OnClickListener(){
			@Override
			public void onClick(View arg0) {
				if(m_gameStart == true)
				{
					new AlertDialog.Builder(MainActivity.this).setTitle("提示")
					.setMessage("要放弃本局吗？").setPositiveButton("确定", new DialogInterface.OnClickListener() {
						@Override
						public void onClick(DialogInterface arg0, int arg1){
							BacktoMenu();
						}
					}).setNegativeButton("取消",  new DialogInterface.OnClickListener(){
						@Override
						public void onClick(DialogInterface arg0, int arg1) {}
					}).show();
				}
				else
				{
					BacktoMenu();
				}
			}
		});	
	}
	
	@Override
	public boolean onTouchEvent(MotionEvent event) {
		// TODO Auto-generated method stub
		if (event.getAction() == MotionEvent.ACTION_UP) {
			if(m_wait == false)
			{
				if(myView.AddPiece((int)event.getX(), (int)event.getY()))
				{
					m_wait = true;			//进行等待时间，不接受落子操作
					int res = myChessboard.CheckGameOver();
					if( res > 0){			//游戏结束，或赢或输或平
						OnGameOver("恭喜！你赢了!", res);
						return true;
					}
					if(m_model == 0){		//AI模式
						if(myChessboard.NextMove() == 1){
							res = myChessboard.CheckGameOver();
							if( res > 0){	//游戏结束，或赢或输或平
								OnGameOver("很遗憾，你输了!", res);
								return true;
							}
							m_wait = false;	//AI落子完毕，等待时间结束
						}
						else{				//AI落子失败
							OnGameOver("", 0);
						}
					}
					else{}					//人人模式
				}
				else{						//玩家落子失败
					//PostiveMsg("无效的落子！","提示", "确定");
					return true;
				}
			}
		}
		return true;
	}
	
	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.main, menu);
		return true;
	}
	
	private void OnGameOver(String msg, int res)
	{
		if(res == 0)
			PostiveMsg("AI不玩了！","倒霉！","确定");
		if(res == 1)
			PostiveMsg(msg, "战斗结束", "确定");
		if(res == 2)
			PostiveMsg("和局！", "战斗结束", "确定");
			
		m_wait = true;
		m_gameStart = false;
	}
	
	private void PostiveMsg(String msg, String title, String btnStr)
	{
		new AlertDialog.Builder(MainActivity.this).setTitle(title)
		.setMessage(msg).setPositiveButton(btnStr, new DialogInterface.OnClickListener() {
			@Override
			public void onClick(DialogInterface arg0, int arg1) {}
		}).show();
		/*.setNegativeButton("cancle",  new DialogInterface.OnClickListener(){
			@Override
			public void onClick(DialogInterface arg0, int arg1) {
			}
		}).show();*/
	}
}
