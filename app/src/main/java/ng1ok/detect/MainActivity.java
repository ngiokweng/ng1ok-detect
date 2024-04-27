package ng1ok.detect;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

import ng1ok.detect.databinding.ActivityMainBinding;

public class MainActivity extends AppCompatActivity {

    private static final String TAG = "nglog";

    // Used to load the 'detect' library on application startup.
    static {
        System.loadLibrary("detect");
    }

    private ActivityMainBinding binding;

    private TextView tvCheckCRC32;

    private void initView(){
        Button btnRefresh = binding.btnRefresh;
        tvCheckCRC32 = binding.tvCheckCRC32;

        tvCheckCRC32.setText(checkCRC32());

        btnRefresh.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Log.d(TAG, "onClick: ");
                tvCheckCRC32.setText(checkCRC32());
            }
        });
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        binding = ActivityMainBinding.inflate(getLayoutInflater());
        setContentView(binding.getRoot());

        initView();
    }

    /**
     * A native method that is implemented by the 'detect' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();

    public native String checkCRC32();
}