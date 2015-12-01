package hu.ait.android.weatherapp.adapter;

import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.support.v4.app.FragmentManager;
import android.support.v4.app.FragmentPagerAdapter;
import android.util.Log;

import hu.ait.android.weatherapp.fragments.BasicWeatherFragment;
import hu.ait.android.weatherapp.fragments.WeatherDetailsFragment;

/**
 * Created by emilyold on 11/30/15.
 */
public class CityDetailsAdapter extends FragmentPagerAdapter {
    public CityDetailsAdapter(FragmentManager fragmentManager) {
        super(fragmentManager);
    }

    @Override
    public CharSequence getPageTitle(int position) {
        switch (position) {
            case 0:
                return "Current Weather";
            case 1:
                return "Details";
            default:
                return "Current Weather";
        }
    }

    @Override
    public Fragment getItem(int position) {
        switch (position) {
            case 0:
                return new BasicWeatherFragment();

            case 1:
                return new WeatherDetailsFragment();

            default:
                return new BasicWeatherFragment();

        }
    }

    @Override
    public int getCount() {
        return 2;
    }
}
