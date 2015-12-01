package hu.ait.android.weatherapp.data;

import com.orm.SugarRecord;

import java.io.Serializable;

/**
 * Created by emilyold on 11/30/15.
 */
public class City extends SugarRecord<City> implements Serializable {

    private String name;

    public City(){

    }

    public City(String name) {
        this.name = name;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }
}
