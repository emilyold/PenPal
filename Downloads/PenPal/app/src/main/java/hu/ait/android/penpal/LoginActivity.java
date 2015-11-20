package hu.ait.android.penpal;

import android.content.Intent;
import android.support.v4.app.Fragment;
import android.support.v4.app.FragmentManager;
import android.support.v4.app.FragmentTransaction;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ProgressBar;
import android.widget.Toast;

import com.parse.LogInCallback;
import com.parse.ParseException;
import com.parse.ParseUser;

import butterknife.Bind;
import butterknife.ButterKnife;
import butterknife.OnClick;
import hu.ait.android.penpal.Fragments.LoginFragment;
import hu.ait.android.penpal.Fragments.RegisterFragment;

public class LoginActivity extends AppCompatActivity implements ILoginFragmentHandler{

//    @Bind(R.id.login_progress)
//    ProgressBar login_progress;
//
//    @Bind(R.id.etEmail)
//    EditText etUserName;
//
//    @Bind(R.id.etPassword)
//    EditText etPassword;
//
//    @Bind(R.id.btnLogin)
//    Button btnLogin;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_login);

        showFragment(LoginFragment.TAG);

//        ButterKnife.bind(this);
//        ParseUser.logOut();
    }

//    @OnClick(R.id.btnLogin)
//    protected void loginUser(){
//
//
//        login_progress.setVisibility(View.VISIBLE);
//
//        ParseUser.logInInBackground(etUserName.getText().toString(), etPassword.getText().toString(), new LogInCallback() {
//            @Override
//            public void done(ParseUser user, ParseException e) {
//                login_progress.setVisibility(View.GONE);
//
//                if (e == null) {
//                    Toast.makeText(LoginActivity.this, user.getUsername() + " login successful",
//                            Toast.LENGTH_SHORT).show();
//
//                    Intent intentStartMainActivity = new Intent(
//                            LoginActivity.this, MainActivity.class);
//                    startActivity(intentStartMainActivity);
//                    // close this activity
//                    finish();
//                } else {
//                    Toast.makeText(LoginActivity.this,
//                            "Login failed: " + e.getMessage(),
//                            Toast.LENGTH_SHORT).show();
//                }
//
//            }
//        });
//    }

    public void showFragment(String tag){
        FragmentManager fm = getSupportFragmentManager();

        Fragment newFragment = fm.findFragmentByTag(tag);
        if(newFragment == null){
            if(tag.equals(LoginFragment.TAG)) newFragment = new LoginFragment();
            else if(tag.equals(RegisterFragment.TAG)) newFragment = new RegisterFragment();
        }

        FragmentTransaction transaction = fm.beginTransaction();

        //must come before replace
        transaction.setCustomAnimations(
                android.R.anim.slide_in_left,
                android.R.anim.slide_out_right);

        //navigate between fragments with back button
        transaction.replace(R.id.layoutContent, newFragment, tag);
        transaction.addToBackStack(null);
        transaction.commit();
    }

    public void showRegisterFragment(){
        showFragment(RegisterFragment.TAG);
    }



}
