class CreateTranslationFiles < ActiveRecord::Migration
  def change
    create_table :translation_files do |t|
      t.references :repository
      t.text :filepath

      t.timestamps
    end
    add_index :translation_files, :repository_id
  end
end
