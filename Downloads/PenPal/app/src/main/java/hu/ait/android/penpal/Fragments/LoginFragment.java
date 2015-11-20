package hu.ait.android.penpal.Fragments;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.support.annotation.Nullable;
import android.support.v4.app.Fragment;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ProgressBar;
import android.widget.TextView;
import android.widget.Toast;

import com.parse.LogInCallback;
import com.parse.ParseException;
import com.parse.ParseUser;

import butterknife.Bind;
import butterknife.ButterKnife;

import butterknife.OnClick;
import hu.ait.android.penpal.ILoginFragmentHandler;
import hu.ait.android.penpal.MainActivity;
import hu.ait.android.penpal.R;

/**
 * Created by emilyold on 11/20/15.
 */
public class LoginFragment extends Fragment {

    public static final String TAG = "LOGIN_FRAGMENT";
    private ILoginFragmentHandler iLoginFragmentHandler;


    @Bind(R.id.login_progress)
    ProgressBar login_progress;

    @Bind(R.id.etEmail)
    EditText etUserName;

    @Bind(R.id.etPassword)
    EditText etPassword;

    @Bind(R.id.btnLogin)
    Button btnLogin;

    @Bind(R.id.link_signup)
    TextView etSignup;

    @Override
    public void onAttach(Context context) {
        super.onAttach(context);
        try{
            iLoginFragmentHandler = (ILoginFragmentHandler) context;
        } catch(ClassCastException e){
            throw new RuntimeException();
        }

        //ButterKnife.bind((Activity) getActivity());
        //ParseUser.logOut();
    }

    @Nullable
    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
        final View rootView = inflater.inflate(R.layout.login_fragment, container, false);

        ButterKnife.bind(this, rootView);

        return rootView;
    }

    @OnClick(R.id.btnLogin)
    public void loginUser(View view){

        login_progress.setVisibility(View.VISIBLE);
        if("".equals(etUserName.getText().toString()) || "".equals(etPassword.getText().toString())){
            login_progress.setVisibility(View.GONE);
            btnLogin.setError("Please supply a valid username and password");
        }
        else{
            ParseUser.logInInBackground(etUserName.getText().toString(), etPassword.getText().toString(), new LogInCallback() {
                @Override
                public void done(ParseUser user, ParseException e) {
                    login_progress.setVisibility(View.GONE);

                    if (e == null) {
                        Toast.makeText(getActivity(), user.getUsername() + " login successful",
                                Toast.LENGTH_SHORT).show();

                        Intent intentStartMainActivity = new Intent(
                                getActivity(), MainActivity.class);
                        startActivity(intentStartMainActivity);
                        // close this activity
                        getActivity().finish();
                    } else {
                        Toast.makeText(getActivity(),
                                "Login failed: " + e.getMessage(),
                                Toast.LENGTH_SHORT).show();
                    }

                }
            });
        }
    }

    @OnClick(R.id.link_signup)
    public  void register(View view){
        iLoginFragmentHandler.showRegisterFragment();
    }

    @Override
    public void onDestroyView() {
        super.onDestroyView();
        ButterKnife.unbind(this);
    }
}
