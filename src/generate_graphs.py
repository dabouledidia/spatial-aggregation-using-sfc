import pandas as pd
import matplotlib.pyplot as plt
import numpy as np


file_path = 'expanded_homicide_data_fixed.csv'
data = pd.read_csv(file_path, encoding='latin1', low_memory=False)


print("First few rows of the data:\n", data.head())


data.columns = ['UID', 'Year', 'Last Name', 'First Name', 'Race', 'Age', 'Gender',
                'City', 'State', 'Latitude', 'Longitude', 'Disposition']


data['Age'] = pd.to_numeric(data['Age'], errors='coerce')
data['Longitude'] = pd.to_numeric(data['Longitude'], errors='coerce')
data['Latitude'] = pd.to_numeric(data['Latitude'], errors='coerce')


data['Disposition'] = data['Disposition'].str.strip()



def plot_age_distribution(data):
    if 'Age' in data.columns:
        plt.figure(figsize=(10, 6))
        data['Age'].dropna().plot(kind='hist', bins=30, edgecolor='black')
        plt.title('Distribution of Victim Age')
        plt.xlabel('Age')
        plt.ylabel('Frequency')
        plt.grid(True)
        plt.show()
    else:
        print("Column 'Age' is missing. Skipping age distribution plot.")

def plot_race_distribution(data):
    if 'Race' in data.columns:
        plt.figure(figsize=(10, 5))
        data['Race'].dropna().value_counts().plot(kind='bar', color='skyblue', edgecolor='black')
        plt.title('Distribution of Victim Race')
        plt.xlabel('Race')
        plt.ylabel('Frequency')
        plt.grid(True)
        plt.show()
    else:
        print("Column 'Race' is missing. Skipping race distribution plot.")

def plot_long_lat_scatter(data):
    if 'Longitude' in data.columns and 'Latitude' in data.columns:
        plt.figure(figsize=(10, 5))
        valid_data = data.dropna(subset=['Longitude', 'Latitude'])  # Ensure lon/lat are not NaN
        plt.scatter(valid_data['Longitude'], valid_data['Latitude'], alpha=0.5, edgecolor='k')
        plt.title('Geographic Distribution of Incidents')
        plt.xlabel('Longitude')
        plt.ylabel('Latitude')
        plt.grid(True)
        plt.show()
    else:
        print("Columns 'Longitude' and/or 'Latitude' are missing. Skipping geographic distribution plot.")

def plot_age_by_race(data):
    if 'Age' in data.columns and 'Race' in data.columns:
        plt.figure(figsize=(12, 8))
        valid_data = data.dropna(subset=['Age', 'Race'])
        valid_data.boxplot(column='Age', by='Race', grid=True)
        plt.title('Victim Age by Race')
        plt.suptitle('')
        plt.xlabel('Race')
        plt.ylabel('Age')
        plt.xticks(rotation=45)
        plt.grid(True)
        plt.show()
    else:
        print("Columns 'Age' and/or 'Race' are missing. Skipping age by race plot.")

def plot_disposition_distribution(data):
    if 'Disposition' in data.columns:
        plt.figure(figsize=(10, 6))
        disposition_data = data['Disposition'].dropna().replace('', np.nan).dropna()
        disposition_data.value_counts().plot(kind='bar', color='lightgreen', edgecolor='black')
        plt.title('Distribution of Disposition')
        plt.xlabel('Disposition')
        plt.ylabel('Frequency')
        plt.grid(True)
        plt.show()
    else:
        print("Column 'Disposition' is missing. Skipping disposition distribution plot.")

def plot_age_by_disposition(data):
    if 'Age' in data.columns and 'Disposition' in data.columns:
        plt.figure(figsize=(12, 8))
        valid_data = data.dropna(subset=['Age', 'Disposition'])
        valid_data.boxplot(column='Age', by='Disposition', grid=True)
        plt.title('Victim Age by Disposition')
        plt.suptitle('')
        plt.xlabel('Disposition')
        plt.ylabel('Age')
        plt.xticks(rotation=45)
        plt.grid(True)
        plt.show()
    else:
        print("Columns 'Age' and/or 'Disposition' are missing. Skipping age by disposition plot.")


plot_age_distribution(data)
plot_race_distribution(data)
plot_long_lat_scatter(data)
plot_age_by_race(data)

