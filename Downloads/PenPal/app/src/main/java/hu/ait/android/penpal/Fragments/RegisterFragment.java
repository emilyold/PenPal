package hu.ait.android.penpal.Fragments;

import android.app.Dialog;
import android.os.Bundle;
import android.support.annotation.NonNull;
import android.support.annotation.Nullable;
import android.support.v4.app.DialogFragment;
import android.support.v4.app.Fragment;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ProgressBar;
import android.widget.Toast;

import com.parse.ParseException;
import com.parse.ParseUser;
import com.parse.SignUpCallback;

import butterknife.Bind;
import butterknife.ButterKnife;
import butterknife.OnClick;
import hu.ait.android.penpal.R;

/**
 * Created by emilyold on 11/20/15.
 */
public class RegisterFragment extends Fragment{

    public static final String TAG = "DialogFragmentRegister";



    @Bind(R.id.input_email)
    EditText etEmail;

    @Bind(R.id.input_name)
    EditText etName;

    @Bind(R.id.input_password)
    EditText etPassword;

    @Bind(R.id.btnRegister)
    Button btnLogin;

    @Nullable
    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
        View rootView = inflater.inflate(R.layout.register_fragment, container, false);
        ButterKnife.bind(this, rootView);
        return rootView;
    }

    @OnClick(R.id.btnRegister)
    protected void registerUser() {
        ParseUser user = new ParseUser();
        user.setUsername(etName.getText().toString());
        user.setUsername(etEmail.getText().toString());
        user.setEmail(etEmail.getText().toString());
        user.setPassword(etPassword.getText().toString());


        user.signUpInBackground(new SignUpCallback() {
            @Override
            public void done(ParseException e) {

                if (e == null) {
                    Toast.makeText(getActivity(), "Registration OK",
                            Toast.LENGTH_SHORT).show();
                    getFragmentManager().popBackStack();
                } else {
                    Toast.makeText(getActivity(),
                            "Registration failed: " + e.getMessage(),
                            Toast.LENGTH_SHORT).show();
                }
            }
        });
    }


}
