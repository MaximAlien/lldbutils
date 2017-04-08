package makhun.com.testapplication;

import android.content.Intent;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;

public class MainActivity extends AppCompatActivity {

    private TestApplication testApplication;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        testApplication = (TestApplication) getApplication();
        testApplication.onActivityCreated(this, savedInstanceState);

        init();

        Button openSecondaryActivityButton = (Button)findViewById(R.id.secondaryActivityButton);
        openSecondaryActivityButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent(MainActivity.this, SecondaryActivity.class);
                startActivity(intent);
            }
        });
    }

    public boolean fillFields(String firstname, String lastname) {
        if (TestApplication.currentActivity != null && TestApplication.currentActivity.getClass() == MainActivity.class) {
            EditText firstnameEditText = (EditText) findViewById(R.id.firstname_editText);
            firstnameEditText.setText(firstname);

            EditText lastnameEditText = (EditText) findViewById(R.id.lastname_editText);
            lastnameEditText.setText(lastname);

            return true;
        }

        return false;
    }

    public native void init();

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }
}
