package hu.ait.android.weatherapp.fragments;

import android.app.Activity;
import android.app.AlertDialog;
import android.app.Dialog;
import android.content.DialogInterface;
import android.os.Bundle;
import android.support.v4.app.DialogFragment;
import android.support.v4.app.Fragment;
import android.view.View;
import android.widget.EditText;

import hu.ait.android.weatherapp.R;
import hu.ait.android.weatherapp.adapter.CityRecyclerAdapter;

/**
 * Created by emilyold on 11/30/15.
 */
public class AddCityFragment extends DialogFragment{



    public interface AddCityListener{
        void onDialogAdd(DialogFragment dialog);
    }

    public static final String TAG = "AddCityDialogTag";
    AddCityListener listener;


    @Override
    public void onAttach(Activity activity) {
        super.onAttach(activity);

        try{
            listener = (AddCityListener) activity;
        }
        catch (ClassCastException e){
            throw new ClassCastException("Must implement AddCityListener interface");
        }
    }

    public Dialog onCreateDialog(Bundle savedInstanceState) {

        View addCityLayout = View.inflate(getActivity(), R.layout.add_city_dialog, null);

        AlertDialog.Builder alertDialogBuilder =
                new AlertDialog.Builder(
                        getActivity());
        alertDialogBuilder.setView(addCityLayout);
        alertDialogBuilder.setTitle("Add a new city");
        alertDialogBuilder.setPositiveButton("OK",
                new DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(DialogInterface dialog, int which) {
                        listener.onDialogAdd(AddCityFragment.this);
                        dialog.dismiss();
                        // if the dialog always dismisses then override this
                        // event handler also in the onStart(..)

                    }
                });
        alertDialogBuilder.setNegativeButton("Cancel",
                new DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(DialogInterface dialog, int which) {
                        dialog.dismiss();
                        // if the dialog always dismisses then override this
                        // event handler also in the onStart(..)

                    }
                });


        return alertDialogBuilder.create();
    }




}
