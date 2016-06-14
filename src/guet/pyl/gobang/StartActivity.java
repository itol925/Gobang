package guet.pyl.gobang;


import guet.pyl.gobang.util.SystemUiHider;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.ImageButton;
import android.widget.RadioGroup;

/**
 * An example full-screen activity that shows and hides the system UI (i.e.
 * status bar and navigation/system bar) with user interaction.
 * 
 * @see SystemUiHider
 */
public class StartActivity extends Activity {
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);

		setContentView(R.layout.activity_start);
		bindClkLisOnStartLayout();
	}
	private void bindClkLisOnStartLayout()
	{
		ImageButton btn_ai = (ImageButton)findViewById(R.id.btn_AI);
		btn_ai.setOnClickListener(new OnClickListener(){
			@Override
			public void onClick(View arg0) {
				JumpToAISetLayout();
			}
		});
		ImageButton btn_player = (ImageButton)findViewById(R.id.btn_player);
		btn_player.setOnClickListener(new OnClickListener(){
			@Override
			public void onClick(View arg0) {
			}
		});
	}
	
	//ai set
	private void JumpToAISetLayout()
	{
		setContentView(R.layout.ai_set);
		
		ImageButton btn = (ImageButton)findViewById(R.id.btn_ai_backToStart);
		btn.setOnClickListener(new OnClickListener(){
			@Override
			public void onClick(View arg0) {
				JumpToStartLayout();
			}
		});	
		ImageButton btn2 = (ImageButton)findViewById(R.id.btn_ai_StartGame);
		btn2.setOnClickListener(new OnClickListener(){
			@Override
			public void onClick(View arg0) {
				Intent intent = new Intent();
				intent.setClass(StartActivity.this, MainActivity.class);
				
				RadioGroup grp = (RadioGroup)findViewById(R.id.ai_radioGroup);
				int difficult = 2;
				switch(grp.getCheckedRadioButtonId())
				{
				case R.id.rb_esay:
					difficult = 8;
					break;
				case R.id.rb_medium:
					difficult = 9;
					break;
				case R.id.rb_crazy:
					difficult = 10;
					break;
				default:
					break;
				}
				
				RadioGroup grp2 = (RadioGroup)findViewById(R.id.ai_radioGroup_color);
				int selectColor = 0;
				switch(grp2.getCheckedRadioButtonId())
				{
				case R.id.rb_black:
					selectColor = 0;
					break;
				case R.id.rb_white:
					selectColor = 1;
					break;
				default:
					break;
				}
				
				RadioGroup grp3 = (RadioGroup)findViewById(R.id.ai_radioGroup_chessboard);
				int selectChessboard = 15;
				switch(grp3.getCheckedRadioButtonId())
				{
				case R.id.rb_9x9:
					selectChessboard = 9;
					break;
				case R.id.rb_12x12:
					selectChessboard = 12;
					break;
				case R.id.rb_15x15:
					selectChessboard = 15;
					break;
				default:
					break;
				}
				//可通过bundle对象传值到下一个Activity
				Bundle bun = new Bundle();
				bun.putInt("model", 0);
				bun.putInt("difficult", difficult);
				bun.putInt("color", selectColor);
				bun.putInt("chessboard", selectChessboard);
				intent.putExtras(bun);
				
				startActivity(intent);
				StartActivity.this.finish();
			}
		});	
		
	}
	private void JumpToStartLayout()
	{
		setContentView(R.layout.activity_start);
		bindClkLisOnStartLayout();
	}
}
