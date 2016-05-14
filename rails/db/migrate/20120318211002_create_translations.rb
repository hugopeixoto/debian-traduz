class CreateTranslations < ActiveRecord::Migration
  def change
    create_table :translations do |t|
      t.references :translation_file
      t.text :msgid
      t.text :msgstr

      t.timestamps
    end
    add_index :translations, :translation_file_id
  end
end
