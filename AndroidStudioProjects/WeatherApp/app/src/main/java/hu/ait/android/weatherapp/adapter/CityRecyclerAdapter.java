package hu.ait.android.weatherapp.adapter;

import android.content.Context;
import android.content.Intent;
import android.graphics.Canvas;
import android.graphics.ColorFilter;
import android.graphics.drawable.Drawable;
import android.support.v7.widget.RecyclerView;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.CheckBox;
import android.widget.ImageButton;
import android.widget.ImageView;
import android.widget.TextView;
import android.widget.Toast;

import java.util.Collections;
import java.util.List;

import hu.ait.android.weatherapp.CityDetails;
import hu.ait.android.weatherapp.MainActivity;
import hu.ait.android.weatherapp.R;
import hu.ait.android.weatherapp.data.City;

/**
 * Created by emilyold on 11/30/15.
 */
public class CityRecyclerAdapter extends RecyclerView.Adapter<CityRecyclerAdapter.ViewHolder> {

    public static final String KEY_CITY_NAME = "CITY_NAME";

    public static class ViewHolder extends RecyclerView.ViewHolder {
        public TextView tvName;
        public ImageButton btnDelete;

        public ViewHolder(View itemView) {
            super(itemView);

            tvName = (TextView) itemView.findViewById(R.id.tvCityName);
            btnDelete = (ImageButton) itemView.findViewById(R.id.btnDelete);
        }
    }

    private List<City> citiesList;
    private Context context;

    public CityRecyclerAdapter(List<City> citiesList, Context context) {
        this.citiesList = citiesList;
        this.context = context;
    }


    @Override
    public ViewHolder onCreateViewHolder(ViewGroup viewGroup, int i) {
        View v = LayoutInflater.from(viewGroup.getContext())
                .inflate(R.layout.city_row, viewGroup, false);
        if((getItemCount()+1) % 2 == 0){

            v.setBackgroundResource(R.drawable.row_background);
        }
        else{
            v.setBackgroundResource(R.drawable.row_background_alternate);
        }

        ViewHolder vh = new ViewHolder(v);
        return vh;
    }

    @Override
    public void onBindViewHolder(final ViewHolder viewHolder, final int i) {
        viewHolder.tvName.setText(citiesList.get(i).getName());
        viewHolder.tvName.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent weatherInfoIntent = new Intent();
                weatherInfoIntent.putExtra(KEY_CITY_NAME, viewHolder.tvName.getText().toString().replaceAll("\\s+", ""));
                weatherInfoIntent.setClass(context, CityDetails.class);
                context.startActivity(weatherInfoIntent);

            }
        });

        viewHolder.btnDelete.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                removeItem(i);
            }
        });
    }

    @Override
    public int getItemCount() {
        return citiesList.size();
    }

    public void addCity(City place) {
        place.save();
        citiesList.add(place);
        notifyDataSetChanged();
    }

    public void updatePlace(int index, City place) {
        citiesList.set(index, place);
        notifyDataSetChanged();
    }

    public void removeItem(int index) {
        // remove it from the DB
        citiesList.get(index).delete();
        // remove it from the list
        citiesList.remove(index);
        notifyDataSetChanged();
    }

    public City getItem(int i) {
        return citiesList.get(i);
    }

}
