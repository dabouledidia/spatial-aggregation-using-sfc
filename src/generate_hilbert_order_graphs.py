import pandas as pd
import re
import matplotlib.pyplot as plt


def parse_percentages(percent_str):
    percent_str = percent_str.strip('[]')
    percent_dict = {}
    for item in percent_str.split(','):
        race, percent = item.split(':')
        race = race.strip()
        percent = float(percent.strip().replace('%', ''))
        percent_dict[race] = percent
    return percent_dict


def clean_data(file_path):
    data = []
    with open(file_path, 'r') as file:
        next(file) 
        for line in file:
            parts = line.strip().split(',')
            hilbert_order = int(parts[0])
            points_count = int(parts[1])
            max_x = float(parts[2])
            min_x = float(parts[3])
            avg_x = float(parts[4])
            max_y = float(parts[5])
            min_y = float(parts[6])
            avg_y = float(parts[7])
            max_age = float(parts[8])
            min_age = float(parts[9])
            avg_age = float(parts[10])
            race_str = ','.join(parts[11:])
            race_percentages = parse_percentages(race_str)
            data.append([hilbert_order, race_percentages, max_age, min_age, avg_age])
    
    return pd.DataFrame(data, columns=['hilbert_order', 'race_percentage', 'max_age', 'min_age', 'avg_age'])


cleaned_df = clean_data('hilbert_order_stats.csv')  # Replace with the correct file path


cleaned_df['max_age'] = pd.to_numeric(cleaned_df['max_age'], errors='coerce')
cleaned_df['min_age'] = pd.to_numeric(cleaned_df['min_age'], errors='coerce')
cleaned_df['avg_age'] = pd.to_numeric(cleaned_df['avg_age'], errors='coerce')


def prepare_plot_data(df, column):
    plot_data = pd.DataFrame(df[column].tolist()).fillna(0)
    plot_data['hilbert_order'] = df['hilbert_order']
    plot_data = plot_data.set_index('hilbert_order')
    return plot_data


def plot_age_distribution(df):
    plt.figure(figsize=(12, 6))
    df.set_index('hilbert_order')[['max_age', 'min_age', 'avg_age']].plot(kind='bar', stacked=False)
    plt.xlabel('Hilbert Order')
    plt.ylabel('Age')
    plt.title('Age Distribution by Hilbert Order')
    plt.legend(['Max Age', 'Min Age', 'Avg Age'])
    plt.show()

def plot_race_percentages(df):
    race_data = prepare_plot_data(df, 'race_percentage')
    race_data.plot(kind='bar', stacked=True, figsize=(12, 6))
    plt.xlabel('Hilbert Order')
    plt.ylabel('Percentage')
    plt.title('Race Percentages by Hilbert Order')
    plt.legend(title='Race')
    plt.xticks(rotation=45)
    plt.show()


plot_age_distribution(cleaned_df)
plot_race_percentages(cleaned_df)

print(cleaned_df)
