package makhun.com.testapplication;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
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
